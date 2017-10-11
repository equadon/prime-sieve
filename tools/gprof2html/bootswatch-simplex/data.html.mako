<%inherit file="main.html.mako" />
<div class="bs-component">
  <ul class="nav nav-tabs">
   %if flat_profile:
     <li class="active"><a href="#flat-profile" data-toggle="tab">Flat profile</a></li>
   %else:
     <li class="disabled"><a>Flat profile: Disabled</a></li>
   %endif
   %if call_graph:
     <li${" active" if not flat_profile else ""}><a href="#call-graph" data-toggle="tab">Call Graph</a></li>
   %else:
     <li class="disabled"><a>Call graph: Disabled</a></li>
   %endif
   %if index:
     <li${" active" if not flat_profile and not call_graph else ""}><a href="#index" data-toggle="tab">Index</a></li>
   %else:
     <li class="disabled"><a>Index: Disabled</a></li>
   %endif
  </ul>
  <div id="profilingTabs" class="tab-content">
   %if flat_profile:
    <div class="tab-pane fade active in" id="flat-profile">
      <table class="table table-hover table-striped table-bordered">
        <thead>
          <tr>
           %for name, desc in flat_profile["header"]:
            <th><abbr title="${desc}">${name}</abbr></th>
           %endfor
          </tr>
        </thead>
        <tbody>
         %for row in flat_profile["body"]:
          <tr>
           %for value in row[:-1]:
            <td>${value if value else '-'}</td>
           %endfor
           <td><a href="#gprof-call-${row[-1]}">${row[-1]}</a></td>
          </tr>
         %endfor
        </tbody>
        <tfoot>
          <tr>
            <td colspan="7">${flat_profile["footer"]}</td>
          </tr>
        </tfoot>
      </table>
    </div>
   %endif
   %if call_graph:
    <div class="tab-pane fade${" active" if not flat_profile else ""} in" id="call-graph">
      ${call_graph_svg}
      <pre>${call_graph}</pre>
    </div>
   %endif
   %if index:
     <div class="tab-pane fade${" active" if not flat_profile and not call_graph else ""} in" id="index">
       <pre>${index_text}</pre>
     </div>
   %endif
  </div>
</div>
