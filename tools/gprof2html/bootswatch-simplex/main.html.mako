<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <title>gprof2html - GProf HTML output</title>
  <link rel="stylesheet" href="bootstrap.min.css" media="screen" />
</head>
<body>
<div class="navbar navbar-default navbar-fixed-top">
  <div class="container">
    <div class="navbar-header">
      <a href="" class="navbar-brand">gprof2html</a>
      <button class="navbar-toggle" type="button" data-toggle="collapse" data-target="#navbar-main">
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
      </button>
    </div>
    <div class="navbar-collapse collapse" id="navbar-main">
      <ul class="nav navbar-nav">
        <li><a href="#flat-profile">Flat profile</a></li>
        <li><a href="#call-graph">Call graph</a></li>
        <li><a href="#index">Index</a></li>
      </ul>
      <ul class="nav navbar-nav navbar-right">
        <li><a href="https://github.com/equadon/gprof2html" target="_blank">GitHub</a></li>
      </ul>
    </div>
  </div>
</div>

<div class="container">
  <div class="page-header" id="banner">
    <div class="row">
      <div class="col-lg-12">
        <h1>gprof2html</h1>
        <p class="lead">HTML output of GNU profiling tool</p>
      </div>
    </div>

    <div class="row">
      <div class="col-lg-8">
${self.body()}
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
</div>

<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.0/jquery.min.js"></script>
<script src="https://netdna.bootstrapcdn.com/bootstrap/3.1.1/js/bootstrap.min.js"></script>
</body>
</html>
