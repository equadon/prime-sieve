#!/usr/bin/env python
# -*-coding:utf-8-*-
from __future__ import print_function

import argparse
import graphviz as gv
import logging
import os
import re
import subprocess
import sys

from mako.template import Template
from mako.lookup import TemplateLookup
from textwrap import dedent


DIR = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__))))

log = logging.getLogger('gprof2html')


def path_exists(p):
    if not os.path.exists(p):
        raise argparse.ArgumentTypeError('%s does not exist' % p)
    return p


def setup():
    # logging
    log.setLevel(logging.DEBUG)

    # file handler
    fh = logging.FileHandler('gprof2html.log')
    fh.setLevel(logging.DEBUG)

    # console handler
    ch = logging.StreamHandler()
    ch.setLevel(logging.DEBUG)

    # log msg formatter
    formatter = logging.Formatter('[%(asctime)s] %(levelname)-7s (%(name)s) %(message)s', datefmt='%H:%M:%S')
    fh.setFormatter(formatter)
    ch.setFormatter(formatter)

    logging.getLogger('').addHandler(fh)
    logging.getLogger('').addHandler(ch)

    # command line arguments
    program = os.path.basename(sys.argv[0])

    parser = argparse.ArgumentParser(description='Generate HTML from GProf output.', version='%s v0.1' % program)
    parser.add_argument('gprof_txt', metavar='gprof.txt', type=path_exists, help='text output from gprof')
    parser.add_argument('-o', '--output', metavar='DIR', type=path_exists, help='output directory')
    parser.add_argument('-t', '--template', metavar='DIR', default='bootswatch-simplex', help='HTML theme (default: bootswatch-simplex)')
    parser.add_argument('-p', '--partial', action='store_true', help='generate partial HTML')

    args = parser.parse_args()

    return args


def build_index(output):
    index = dict(id=dict(), func=dict())
    for m in re.finditer(r'\[(\d+)\]([^\[]+)', output):
        id = '[%d]' % int(m.group(1))
        func = m.group(2).strip()
        name = 'gprof-func-%s' % func
        index['id'][id] = (func, name)
        index['func'][func] = name

    return index


def update_index(index, output):
    if index is None or output is None:
        return output
    for func, name in index['func'].items():
        output = output.replace(' %s' % func, ' <a class="gprof-index" href="#%s">%s</a> ' % (name, func))
    for id, (func, name) in index['id'].items():
        output = output.replace(id, '<a class="gprof-index" href="#%s"><abbr title="%s">%s</abbr></a>' % (name, func, id))
    return output


def title_first_letter(s):
    return s[0].upper() + s[1:]


def parse_flat_profile(output):
    table_output = output[0].strip()
    descriptions = output[1].replace('\t', ' '*8)

    table = dict(
        header=[],
        body=[],
        footer=''
    )

    re_line = re.compile(r'(.{11})\s*(.*)')

    # parse descriptions
    name = []
    desc = ''
    for d in descriptions.split('\n'):
        m = re_line.search(d)
        if m:
            desc = '%s %s' % (desc, m.group(2).strip())
            if len(name) < 2:
                name.append(m.group(1).strip())
            else:
                desc = '%s %s' % (desc, m.group(1).strip())
        else:
            if name:
                table['header'].append((' '.join(name).strip().title(), title_first_letter(desc.strip())))
            name = []
            desc = ''

    # collect table data (hard code header for now)
    re_data = re.compile(r'([\d\.]+)\s+([\d\.]+)\s+([\d\.]+)\s+(\d+)?\s+([\d\.]+)?\s+([\d\.]+)?\s+(.*)')
    lines = table_output.split('\n')
    for line in lines:
        index = lines.index(line)
        if index == 0:
            table['footer'] = line.strip()
            continue
        m = re_data.search(line)
        if m:
            table['body'].append(m.groups())

    return table


def node_name(node, children_time):
    if children_time > 0:
        return '%s(): %s s' % (node['name'], node['children_time'])
    return '%s(): %s s' % (node['name'], node['self_time'])


def build_graphviz_nodes(g, funcs, node, parent):
    try:
        children_time = float(node['children_time'])
        self_time = float(node['self_time'])
    except ValueError:
        children_time = -1
        self_time = -1

    if parent is None or self_time > 0:
        g.node(node['name'], label=node_name(node, children_time))

        if parent is not None:
            g.edge(parent['name'], node['name'])#, label='%s s' % node['self_time'])

        if children_time > 0:
            for child in node['children']:
                build_graphviz_nodes(g, funcs, funcs[child], node)


def dot_styles(dot):
    dot.graph_attr.update(bgcolor='#ffffff00')
    dot.node_attr.update(fontname='Source Code Pro', fontsize='10', style='filled', shape='oval')
    dot.edge_attr.update(fontname='Ubuntu', fontsize='12', arrowhead='open')


def parse_functions(output):
    funcs = dict()

    #re_parent = re.compile(r'\[(\d+)\]\s+([\d\.]+)?\s+([\d\.]+)?\s+([\d\.]+)?\s+([\d\./]+)?\s+(.*)\n')
    re_parent = re.compile(r'\[(\d+)\]\s+([\d\.]+)?\s+([\d\.]+)?\s+([\d\.]+)?\s+([\d\./]+)?\s+([^\[]+)')
    #re_child = re.compile(r'([\d\.]+)?\s+([\d\.]+)?\s+([\d\./]+)?\s+(.*)\n')
    re_child = re.compile(r'([\d\.]+)\s+([\d\.]+)\s+([\d\./]+)\s+([^\[]+)')

    def new_func(name=None, self_time=0, children_time=0, called='', percent=0):
        return dict(
            name=name,
            self_time=self_time,
            children_time=children_time,
            called=called,
            percent=percent,
            children=[],
            parents=[],
        )

    func = new_func()
    for line in output.split('\n'):
        if line.strip() == '' or 'spontaneous' in line:
            continue
        if line.strip().startswith('---'):
            # function done, prepare for next
            funcs[func['name']] = func
            func = new_func()
        elif line.startswith('['):
            # current function
            m = re_parent.search(line)
            func['name'] = m.group(6).strip() if m.group(6) else ''
            func['percent'] = m.group(2).strip() if m.group(2) else ''
            func['self_time'] = m.group(3).strip() if m.group(3) else ''
            func['children_time'] = m.group(4).strip() if m.group(4) else ''
            func['called'] = m.group(5).strip() if m.group(5) else ''
            if func['name'] == 'remove_multiples_bit':
                log.info('%s: self=%s, children=%s' % (func['name'], func['self_time'], func['children_time']))
                log.info(m.groups())
        else:
            m = re_child.search(line)
            if func['name'] is None:
                func['parents'].append(m.group(4).strip())
            else:
                func['children'].append(m.group(4).strip())
    return funcs


def build_call_graph(output):
    re_cg = re.search(r'index % time [^\n]+\n(.*)This table describes', output, re.DOTALL)
    cg_data = re_cg.group(1)
    funcs = parse_functions(cg_data)

    # find root
    root = None
    for key, func in funcs.items():
        if len(func['parents']) == 0:
            root = func
            break

    if root is None:
        print('Error: no root found!')
        exit(1)

    # build graphviz nodes
    dot = gv.Digraph(comment='Call graph', format='svg')
    build_graphviz_nodes(dot, funcs, root, None)
    dot_styles(dot)

    return output, dot.pipe().decode('utf-8')


def gprof_parse(output):
    # build index
    index_m = re.search(r'Index by function name(.*)', output, re.DOTALL)
    index_text = None
    if index_m:
        index_text = dedent(index_m.group(1)).replace('\n\n', '\n')
        index = build_index(index_text)

    # flat profile
    flat_profile_m = re.search(r'Flat profile:.*(Each sample counts.*)\n\n( \%.*)Copyright.*Call graph', output, re.DOTALL)
    flat_profile = None
    if flat_profile_m:
        flat_profile = parse_flat_profile(flat_profile_m.groups())

    # call graph
    call_graph_m = re.search(r'Call graph.*(granularity.*)Copyright', output, re.DOTALL)
    call_graph = None
    if call_graph_m:
        call_graph = call_graph_m.group(1).strip()
        call_graph, call_graph_svg = build_call_graph(call_graph)

    # update urls to indexed functions
    index_text = update_index(index, index_text)
    call_graph = update_index(index, call_graph)

    return dict(
        flat_profile=flat_profile,
        call_graph=call_graph,
        call_graph_svg=call_graph_svg,
        index=index,
        index_text=index_text
    )


def generate_html(template_name, partial, params):
    template_dir = os.path.join(DIR, template_name)

    lookup = TemplateLookup(directories=[template_dir])

    with open(os.path.join(template_dir, 'data.html.mako'), 'r') as f:
        if partial:
            data = f.read().replace('inherit file="main.', 'inherit file="partial.')
        else:
            data = f.read()
    template = Template(data, lookup=lookup)

    return template.render(**params)


def main():
    args = setup()

    with open(args.gprof_txt, 'r') as f:
        gprof_output = gprof_parse(f.read())

    html = generate_html(args.template, args.partial, gprof_output)

    if args.output is None:
        print(html)
    else:
        with open(args.output, 'w+') as f:
            f.write(html)
        log.info('Saved HTML to file: %s' % args.output)


if __name__ == '__main__':
    main()
