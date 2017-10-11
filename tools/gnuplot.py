import logging
import subprocess

from os import path
from mako.template import Template


class GNUPlot(object):
    def __init__(self, filename, template_dir):
        self.log = logging.getLogger('gnuplot')
        self.filename = filename
        self.data_filename = '%s.dat' % self.filename
        self.svg_filename = '%s.svg' % self.filename
        self.data = dict()
        self.data_template = Template(filename=path.join(template_dir, 'plot.dat.mako'))
        self.template = Template(filename=path.join(template_dir, 'plot.gnu.mako'))

    def add_data(self, array_type, x, y):
        if array_type not in self.data:
            self.data[array_type] = []
        self.data[array_type].append((x, y))

    def _save_data(self, name, xlabel, ylabel):
        params = dict(
            name=name,
            xlabel=xlabel,
            ylabel=ylabel,
            bit_data=self.data['bit'],
            bool_data=self.data['bool'],
        )
        with open(self.data_filename, 'w+') as f:
            f.write(self.data_template.render(**params))
        self.log.debug('Saved GNUPlot data to: %s' % path.relpath(self.data_filename))

    def save(self, name, title, xlabel, ylabel, label):
        self._save_data(name, xlabel, ylabel)

        params = dict(
            title='%s scalability - %s' % (name.title(), title),
            svg_filename=path.relpath(self.svg_filename),
            label=label,
            xlabel=xlabel,
            ylabel=ylabel,
            datafile=path.relpath(self.data_filename),
        )

        with open(self.filename, 'w+') as f:
            f.write(self.template.render(**params))
        self.log.debug('Saved GNUPlot to: %s' % path.relpath(self.filename))

        self._generate_svg()

    def _generate_svg(self):
        subprocess.call(['gnuplot', self.filename])

    def render_svg(self):
        with open(self.svg_filename, 'r') as f:
            output = f.read()
        return output
