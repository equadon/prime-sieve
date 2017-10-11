<%!
  def pretty_int(number):
      if number == 'None':
          return '-'
      return '{:,}'.format(int(number)).replace(',', ' ')
  def pretty_float(number):
      if number == 'None':
          return '-'
      return '%.3f' % float(number)
  def title(text):
      return text.title()
  def pretty_memory(memory):
      return '%.1f' % round(float(memory), 1)
%>
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <title>IOOPM: Sieve of Eratosthenes using POSIX Threads</title>
  <style type="text/css">
    @media (min-width: 1200px) {
      .container{
        max-width: none !important;
        width: 90% !important;
      }
    }
    ${bootstrap_css}
    td, th {
      text-align: right !important;
    }
    tr.removed {
      color: #ccc;
      background-color: #fff9f9 !important;
    }
  </style>
</head>
<body>
<div class="navbar navbar-default navbar-fixed-top">
  <div class="container">
    <div class="navbar-header">
      <a href="" class="navbar-brand">IOOPM</a>
      <button class="navbar-toggle" type="button" data-toggle="collapse" data-target="#navbar-main">
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
      </button>
    </div>
    <div class="navbar-collapse collapse" id="navbar-main">
      <ul class="nav navbar-nav">
        <li><a href="#strong">Strong scalability</a></li>
        <li><a href="#weak">Weak scalability</a></li>
        <li><a href="#tests">Test data</a></li>
      </ul>

      <ul class="nav navbar-nav navbar-right">
        <li><a href="https://github.com/IOOPM-UU/niklas.persson.1195" target="_blank">GitHub</a></li>
        <li><a href="http://wrigstad.com/ioopm/" target="_blank">IOOPM</a></li>
      </ul>

    </div>
  </div>
</div>

<div class="container">

  <a name="strong"></a>

  <div class="page-header" id="banner">
    <div class="row">
      <div class="col-lg-6">
        <h1>Sieve of Eratosthenes</h1>
        <p class="lead">Parallel sieve using POSIX Threads</p>
      </div>
    </div>
  </div>

  <div class="bs-docs-section">

    <div class="row">
      <div class="col-lg-12">
        <h2 id="strong">Strong scalability <small>${numtests} test(s) per run</small></h2>
      </div>
    </div>

    <div class="row">

      <div class="col-md-6 col-lg-4">
        <div class="panel panel-success">
          <div class="panel-heading">
            <h3 class="panel-title">Strong scalability - Time</h3>
          </div>
          <div class="panel-body">
            ${plot_time_strong_svg}
          </div>
        </div>
      </div>

      <div class="col-md-6 col-lg-4">
        <div class="panel panel-danger">
          <div class="panel-heading">
            <h3 class="panel-title">Strong scalability - Speedup</h3>
          </div>
          <div class="panel-body">
            ${plot_speedup_strong_svg}
          </div>
        </div>
      </div>

      <div class="col-md-6 col-lg-4">
        <div class="panel panel-warning">
          <div class="panel-heading">
            <h3 class="panel-title">Strong scalability - Memory</h3>
          </div>
          <div class="panel-body">
            ${plot_memory_strong_svg}
          </div>
        </div>
      </div>

    </div>


  </div>

  <a name="weak"></a>

  <div class="bs-docs-section">

    <div class="row">
      <div class="col-lg-12">
        <h2 id="weak">Weak scalability <small>${numtests} test(s) per run</small></h2>
      </div>
    </div>

    <div class="row">

      <div class="col-md-6 col-lg-4">
        <div class="panel panel-success">
          <div class="panel-heading">
            <h3 class="panel-title">Weak scalability - Time</h3>
          </div>
          <div class="panel-body">
            ${plot_time_weak_svg}
          </div>
        </div>
      </div>

      <div class="col-md-6 col-lg-4">
        <div class="panel panel-danger">
          <div class="panel-heading">
            <h3 class="panel-title">Weak scalability - Speedup</h3>
          </div>
          <div class="panel-body">
            ${plot_speedup_weak_svg}
          </div>
        </div>
      </div>

      <div class="col-md-6 col-lg-4">
        <div class="panel panel-warning">
          <div class="panel-heading">
            <h3 class="panel-title">Weak scalability - Memory</h3>
          </div>
          <div class="panel-body">
            ${plot_memory_weak_svg}
          </div>
        </div>
      </div>

    </div>

  </div>

  <div class="bs-docs-section">
    <a name="gprof"></a>

    <div class="row">
      <div class="col-lg-12 bs-component">
        <div class="panel panel-info">
          <div class="panel-heading">
            <h2 class="panel-title">GProf output <small>T = ${gprof_T | pretty_int}</small></h2>
          </div>
          <div class="panel-body">
            ${gprof_tabs}
          </div>
        </div>
      </div
    </div>

    <a name="tests"></a>
    <div class="row">

     % for test_name, data in tests:
      <div class="col-lg-12 bs-component">
        <div class="panel panel-info">
          <div class="panel-heading">
            <h3 class="panel-title">${test_name} scalability - Tests</h3>
          </div>
          <div class="panel-body">
            <table class="table table-hover">
              <thead>
              <tr>
                <th>Test.#</th>
                <th>Array</th>
                <th>Cores</th>
                <th>T</th>
                <th>Primes</th>
                <th>Elapsed</th>
                <th><abbr title="User + system time">Time</abbr></th>
                <th>Speedup</th>
                <th><abbr title="Maximum resident set size">Memory</abbr></th>
              </tr>
              </thead>
              <tbody>
                <%
                  trclass = ''
                  removedclass = ''
                  testnum = 0
                %>
                % for num, type, cores, T, primes, elapsed, time, speedup, memory, removed in data:
                <%
                  if removed:
                      removedclass = 'removed'
                  else:
                      removedclass = ''
                  if num == 1:
                      testnum += 1
                      if trclass == '':
                          trclass = 'active'
                      else:
                          trclass = ''
                %>
                  <tr class="${removedclass} ${trclass if not removed else ''}">
                    <td>
                     % if removed:
                      -
                     % else:
                      ${testnum}.${num}
                     % endif
                    </td>
                    <td>${type | title}</td>
                    <td>${cores}</td>
                    <td>${T | pretty_int}</td>
                    <td>${primes | pretty_int}</td>
                    <td>${elapsed | pretty_float} s</td>
                    <td>${time | pretty_float} s</td>
                    <td>${speedup | pretty_float}</td>
                    <td>${memory | pretty_memory} MB</td>
                  </tr>
                % endfor
              </tbody>
            </table>
          </div>
        </div>
      </div>
     % endfor
    </div>

  </div>

  <footer>
    <div class="row">
      <div class="col-lg-12">

        <ul class="list-unstyled">
          <li class="pull-right"><a href="#top">Back to top</a></li>
          <li><a href="https://github.com/IOOPM-UU/niklas.persson.1195">GitHub</a></li>
          <li>Made by <a href="http://www.equadon.com/">Niklas Persson</a>.</li>
        </ul>
        <p>Theme made by <a href="https://bootswatch.com/" rel="nofollow">Bootswatch.com</a>
      </div>
    </div>
  </footer>
</div>

</body>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.0/jquery.min.js"></script>
<script src="https://netdna.bootstrapcdn.com/bootstrap/3.1.1/js/bootstrap.min.js"></script>
</html>
