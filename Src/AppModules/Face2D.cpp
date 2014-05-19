




<!DOCTYPE html>
<html class="   ">
  <head prefix="og: http://ogp.me/ns# fb: http://ogp.me/ns/fb# object: http://ogp.me/ns/object# article: http://ogp.me/ns/article# profile: http://ogp.me/ns/profile#">
    <meta charset='utf-8'>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    
    
    <title>magic3d/Src/AppModules/Face2D.cpp at master · magic3dcn/magic3d</title>
    <link rel="search" type="application/opensearchdescription+xml" href="/opensearch.xml" title="GitHub" />
    <link rel="fluid-icon" href="https://github.com/fluidicon.png" title="GitHub" />
    <link rel="apple-touch-icon" sizes="57x57" href="/apple-touch-icon-114.png" />
    <link rel="apple-touch-icon" sizes="114x114" href="/apple-touch-icon-114.png" />
    <link rel="apple-touch-icon" sizes="72x72" href="/apple-touch-icon-144.png" />
    <link rel="apple-touch-icon" sizes="144x144" href="/apple-touch-icon-144.png" />
    <meta property="fb:app_id" content="1401488693436528"/>

      <meta content="@github" name="twitter:site" /><meta content="summary" name="twitter:card" /><meta content="magic3dcn/magic3d" name="twitter:title" /><meta content="magic3d - Magic3D is a platform for developing graphics and vision related cutting-edge technology, including but not limited: 3D scanning, reconstruction, modeling, image processing." name="twitter:description" /><meta content="https://avatars3.githubusercontent.com/u/6185106?s=400" name="twitter:image:src" />
<meta content="GitHub" property="og:site_name" /><meta content="object" property="og:type" /><meta content="https://avatars3.githubusercontent.com/u/6185106?s=400" property="og:image" /><meta content="magic3dcn/magic3d" property="og:title" /><meta content="https://github.com/magic3dcn/magic3d" property="og:url" /><meta content="magic3d - Magic3D is a platform for developing graphics and vision related cutting-edge technology, including but not limited: 3D scanning, reconstruction, modeling, image processing." property="og:description" />

    <link rel="assets" href="https://assets-cdn.github.com/">
    <link rel="conduit-xhr" href="https://ghconduit.com:25035/">
    <link rel="xhr-socket" href="/_sockets" />

    <meta name="msapplication-TileImage" content="/windows-tile.png" />
    <meta name="msapplication-TileColor" content="#ffffff" />
    <meta name="selected-link" value="repo_source" data-pjax-transient />
    <meta content="collector.githubapp.com" name="octolytics-host" /><meta content="collector-cdn.github.com" name="octolytics-script-host" /><meta content="github" name="octolytics-app-id" /><meta content="70413EFA:3040:A2A1B20:5379F589" name="octolytics-dimension-request_id" /><meta content="6185670" name="octolytics-actor-id" /><meta content="liguocn" name="octolytics-actor-login" /><meta content="ae2cdcc2cad20d6cb26b862a0d98585e6d3e956a9ed4a28545006cc3f655d3c6" name="octolytics-actor-hash" />
    

    
    
    <link rel="icon" type="image/x-icon" href="https://assets-cdn.github.com/favicon.ico" />

    <meta content="authenticity_token" name="csrf-param" />
<meta content="Zl20WziG8ePYzUD35owwNrJkC/rHAMaLsuLWyz/PQAVP3qeU4Uu7Y9YrzLtTQvfgaLFJ3+XsoaM1Cma0/aLXsA==" name="csrf-token" />

    <link href="https://assets-cdn.github.com/assets/github-da7475c114a3c2eab7b91a0584202a89fd188935.css" media="all" rel="stylesheet" type="text/css" />
    <link href="https://assets-cdn.github.com/assets/github2-a49192fef33ef6e2053a634597427056eb62d634.css" media="all" rel="stylesheet" type="text/css" />
    


    <meta http-equiv="x-pjax-version" content="e22dc102a1df9d45c9f2153cbc5fc5b0">

      
  <meta name="description" content="magic3d - Magic3D is a platform for developing graphics and vision related cutting-edge technology, including but not limited: 3D scanning, reconstruction, modeling, image processing." />

  <meta content="6185106" name="octolytics-dimension-user_id" /><meta content="magic3dcn" name="octolytics-dimension-user_login" /><meta content="15197513" name="octolytics-dimension-repository_id" /><meta content="magic3dcn/magic3d" name="octolytics-dimension-repository_nwo" /><meta content="true" name="octolytics-dimension-repository_public" /><meta content="false" name="octolytics-dimension-repository_is_fork" /><meta content="15197513" name="octolytics-dimension-repository_network_root_id" /><meta content="magic3dcn/magic3d" name="octolytics-dimension-repository_network_root_nwo" />
  <link href="https://github.com/magic3dcn/magic3d/commits/master.atom" rel="alternate" title="Recent Commits to magic3d:master" type="application/atom+xml" />

  </head>


  <body class="logged_in  env-production macintosh vis-public page-blob">
    <a href="#start-of-content" tabindex="1" class="accessibility-aid js-skip-to-content">Skip to content</a>
    <div class="wrapper">
      
      
      
      


      <div class="header header-logged-in true">
  <div class="container clearfix">

    <a class="header-logo-invertocat" href="https://github.com/">
  <span class="mega-octicon octicon-mark-github"></span>
</a>

    
    <a href="/notifications" aria-label="You have no unread notifications" class="notification-indicator tooltipped tooltipped-s" data-hotkey="g n">
        <span class="mail-status all-read"></span>
</a>

      <div class="command-bar js-command-bar  in-repository">
          <form accept-charset="UTF-8" action="/search" class="command-bar-form" id="top_search_form" method="get">

<div class="commandbar">
  <span class="message"></span>
  <input type="text" data-hotkey="s, /" name="q" id="js-command-bar-field" placeholder="Search or type a command" tabindex="1" autocapitalize="off"
    
    data-username="liguocn"
      data-repo="magic3dcn/magic3d"
      data-branch="master"
      data-sha="3a75f513c4d3962dd8187850758c8fce7bbbf9ba"
  >
  <div class="display hidden"></div>
</div>

    <input type="hidden" name="nwo" value="magic3dcn/magic3d" />

    <div class="select-menu js-menu-container js-select-menu search-context-select-menu">
      <span class="minibutton select-menu-button js-menu-target" role="button" aria-haspopup="true">
        <span class="js-select-button">This repository</span>
      </span>

      <div class="select-menu-modal-holder js-menu-content js-navigation-container" aria-hidden="true">
        <div class="select-menu-modal">

          <div class="select-menu-item js-navigation-item js-this-repository-navigation-item selected">
            <span class="select-menu-item-icon octicon octicon-check"></span>
            <input type="radio" class="js-search-this-repository" name="search_target" value="repository" checked="checked" />
            <div class="select-menu-item-text js-select-button-text">This repository</div>
          </div> <!-- /.select-menu-item -->

          <div class="select-menu-item js-navigation-item js-all-repositories-navigation-item">
            <span class="select-menu-item-icon octicon octicon-check"></span>
            <input type="radio" name="search_target" value="global" />
            <div class="select-menu-item-text js-select-button-text">All repositories</div>
          </div> <!-- /.select-menu-item -->

        </div>
      </div>
    </div>

  <span class="help tooltipped tooltipped-s" aria-label="Show command bar help">
    <span class="octicon octicon-question"></span>
  </span>


  <input type="hidden" name="ref" value="cmdform">

</form>
        <ul class="top-nav">
          <li class="explore"><a href="/explore">Explore</a></li>
            <li><a href="https://gist.github.com">Gist</a></li>
            <li><a href="/blog">Blog</a></li>
          <li><a href="https://help.github.com">Help</a></li>
        </ul>
      </div>

    


  <ul id="user-links">
    <li>
      <a href="/liguocn" class="name">
        <img alt="liguo" class=" js-avatar" data-user="6185670" height="20" src="https://avatars2.githubusercontent.com/u/6185670?s=140" width="20" /> liguocn
      </a>
    </li>

    <li class="new-menu dropdown-toggle js-menu-container">
      <a href="#" class="js-menu-target tooltipped tooltipped-s" aria-label="Create new...">
        <span class="octicon octicon-plus"></span>
        <span class="dropdown-arrow"></span>
      </a>

      <div class="new-menu-content js-menu-content">
      </div>
    </li>

    <li>
      <a href="/settings/profile" id="account_settings"
        class="tooltipped tooltipped-s"
        aria-label="Account settings ">
        <span class="octicon octicon-tools"></span>
      </a>
    </li>
    <li>
      <form class="logout-form" action="/logout" method="post">
        <button class="sign-out-button tooltipped tooltipped-s" aria-label="Sign out">
          <span class="octicon octicon-log-out"></span>
        </button>
      </form>
    </li>

  </ul>

<div class="js-new-dropdown-contents hidden">
  

<ul class="dropdown-menu">
  <li>
    <a href="/new"><span class="octicon octicon-repo-create"></span> New repository</a>
  </li>
  <li>
    <a href="/organizations/new"><span class="octicon octicon-organization"></span> New organization</a>
  </li>


    <li class="section-title">
      <span title="magic3dcn/magic3d">This repository</span>
    </li>
      <li>
        <a href="/magic3dcn/magic3d/issues/new"><span class="octicon octicon-issue-opened"></span> New issue</a>
      </li>
</ul>

</div>


    
  </div>
</div>

      

        



      <div id="start-of-content" class="accessibility-aid"></div>
          <div class="site" itemscope itemtype="http://schema.org/WebPage">
    <div id="js-flash-container">
      
    </div>
    <div class="pagehead repohead instapaper_ignore readability-menu">
      <div class="container">
        

<ul class="pagehead-actions">

    <li class="subscription">
      <form accept-charset="UTF-8" action="/notifications/subscribe" class="js-social-container" data-autosubmit="true" data-remote="true" method="post"><div style="margin:0;padding:0;display:inline"><input name="authenticity_token" type="hidden" value="MnpePfHCI0PkUwEFmmL+r2BOJI0PwZWTuC3IE8lc3w7VPkGcACIolh2+8OyYlIKCiBx3Ow8Rg1A8vWkzDDmIBw==" /></div>  <input id="repository_id" name="repository_id" type="hidden" value="15197513" />

    <div class="select-menu js-menu-container js-select-menu">
      <a class="social-count js-social-count" href="/magic3dcn/magic3d/watchers">
        2
      </a>
      <span class="minibutton select-menu-button with-count js-menu-target" role="button" tabindex="0" aria-haspopup="true">
        <span class="js-select-button">
          <span class="octicon octicon-eye-unwatch"></span>
          Unwatch
        </span>
      </span>

      <div class="select-menu-modal-holder">
        <div class="select-menu-modal subscription-menu-modal js-menu-content" aria-hidden="true">
          <div class="select-menu-header">
            <span class="select-menu-title">Notification status</span>
            <span class="octicon octicon-remove-close js-menu-close"></span>
          </div> <!-- /.select-menu-header -->

          <div class="select-menu-list js-navigation-container" role="menu">

            <div class="select-menu-item js-navigation-item " role="menuitem" tabindex="0">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <div class="select-menu-item-text">
                <input id="do_included" name="do" type="radio" value="included" />
                <h4>Not watching</h4>
                <span class="description">You only receive notifications for conversations in which you participate or are @mentioned.</span>
                <span class="js-select-button-text hidden-select-button-text">
                  <span class="octicon octicon-eye-watch"></span>
                  Watch
                </span>
              </div>
            </div> <!-- /.select-menu-item -->

            <div class="select-menu-item js-navigation-item selected" role="menuitem" tabindex="0">
              <span class="select-menu-item-icon octicon octicon octicon-check"></span>
              <div class="select-menu-item-text">
                <input checked="checked" id="do_subscribed" name="do" type="radio" value="subscribed" />
                <h4>Watching</h4>
                <span class="description">You receive notifications for all conversations in this repository.</span>
                <span class="js-select-button-text hidden-select-button-text">
                  <span class="octicon octicon-eye-unwatch"></span>
                  Unwatch
                </span>
              </div>
            </div> <!-- /.select-menu-item -->

            <div class="select-menu-item js-navigation-item " role="menuitem" tabindex="0">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <div class="select-menu-item-text">
                <input id="do_ignore" name="do" type="radio" value="ignore" />
                <h4>Ignoring</h4>
                <span class="description">You do not receive any notifications for conversations in this repository.</span>
                <span class="js-select-button-text hidden-select-button-text">
                  <span class="octicon octicon-mute"></span>
                  Stop ignoring
                </span>
              </div>
            </div> <!-- /.select-menu-item -->

          </div> <!-- /.select-menu-list -->

        </div> <!-- /.select-menu-modal -->
      </div> <!-- /.select-menu-modal-holder -->
    </div> <!-- /.select-menu -->

</form>
    </li>

  <li>
  

  <div class="js-toggler-container js-social-container starring-container ">

    <form accept-charset="UTF-8" action="/magic3dcn/magic3d/unstar" class="js-toggler-form starred" data-remote="true" method="post"><div style="margin:0;padding:0;display:inline"><input name="authenticity_token" type="hidden" value="SphPl71vNIRH2pgAfsz7EF7AdYimVDLPRah/5sVO8JWPANVUh5p6RbpRgp2bPTBf71oDhbknOitq/i5au5eQQw==" /></div>
      <button
        class="minibutton with-count js-toggler-target star-button"
        aria-label="Unstar this repository" title="Unstar magic3dcn/magic3d">
        <span class="octicon octicon-star-delete"></span><span class="text">Unstar</span>
      </button>
        <a class="social-count js-social-count" href="/magic3dcn/magic3d/stargazers">
          2
        </a>
</form>
    <form accept-charset="UTF-8" action="/magic3dcn/magic3d/star" class="js-toggler-form unstarred" data-remote="true" method="post"><div style="margin:0;padding:0;display:inline"><input name="authenticity_token" type="hidden" value="3r1Gl6UNCFa9XSX3VK0iaTbM3r/Hgb15xdHh5CToc2XzTEk8kH8UQdDNF6Yh2AAE14anlyyh1ZMLtb2zvk4WYQ==" /></div>
      <button
        class="minibutton with-count js-toggler-target star-button"
        aria-label="Star this repository" title="Star magic3dcn/magic3d">
        <span class="octicon octicon-star"></span><span class="text">Star</span>
      </button>
        <a class="social-count js-social-count" href="/magic3dcn/magic3d/stargazers">
          2
        </a>
</form>  </div>

  </li>


        <li>
          <a href="/magic3dcn/magic3d/fork" class="minibutton with-count js-toggler-target fork-button lighter tooltipped-n" title="Fork your own copy of magic3dcn/magic3d to your account" aria-label="Fork your own copy of magic3dcn/magic3d to your account" rel="nofollow" data-method="post">
            <span class="octicon octicon-git-branch-create"></span><span class="text">Fork</span>
          </a>
          <a href="/magic3dcn/magic3d/network" class="social-count">0</a>
        </li>


</ul>

        <h1 itemscope itemtype="http://data-vocabulary.org/Breadcrumb" class="entry-title public">
          <span class="repo-label"><span>public</span></span>
          <span class="mega-octicon octicon-repo"></span>
          <span class="author"><a href="/magic3dcn" class="url fn" itemprop="url" rel="author"><span itemprop="title">magic3dcn</span></a></span><!--
       --><span class="path-divider">/</span><!--
       --><strong><a href="/magic3dcn/magic3d" class="js-current-repository js-repo-home-link">magic3d</a></strong>

          <span class="page-context-loader">
            <img alt="Octocat-spinner-32" height="16" src="https://assets-cdn.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
          </span>

        </h1>
      </div><!-- /.container -->
    </div><!-- /.repohead -->

    <div class="container">
      <div class="repository-with-sidebar repo-container new-discussion-timeline js-new-discussion-timeline  ">
        <div class="repository-sidebar clearfix">
            

<div class="sunken-menu vertical-right repo-nav js-repo-nav js-repository-container-pjax js-octicon-loaders">
  <div class="sunken-menu-contents">
    <ul class="sunken-menu-group">
      <li class="tooltipped tooltipped-w" aria-label="Code">
        <a href="/magic3dcn/magic3d" aria-label="Code" class="selected js-selected-navigation-item sunken-menu-item" data-hotkey="g c" data-pjax="true" data-selected-links="repo_source repo_downloads repo_commits repo_releases repo_tags repo_branches /magic3dcn/magic3d">
          <span class="octicon octicon-code"></span> <span class="full-word">Code</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://assets-cdn.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>

        <li class="tooltipped tooltipped-w" aria-label="Issues">
          <a href="/magic3dcn/magic3d/issues" aria-label="Issues" class="js-selected-navigation-item sunken-menu-item js-disable-pjax" data-hotkey="g i" data-selected-links="repo_issues /magic3dcn/magic3d/issues">
            <span class="octicon octicon-issue-opened"></span> <span class="full-word">Issues</span>
            <span class='counter'>0</span>
            <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://assets-cdn.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>        </li>

      <li class="tooltipped tooltipped-w" aria-label="Pull Requests">
        <a href="/magic3dcn/magic3d/pulls" aria-label="Pull Requests" class="js-selected-navigation-item sunken-menu-item js-disable-pjax" data-hotkey="g p" data-selected-links="repo_pulls /magic3dcn/magic3d/pulls">
            <span class="octicon octicon-git-pull-request"></span> <span class="full-word">Pull Requests</span>
            <span class='counter'>0</span>
            <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://assets-cdn.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>


        <li class="tooltipped tooltipped-w" aria-label="Wiki">
          <a href="/magic3dcn/magic3d/wiki" aria-label="Wiki" class="js-selected-navigation-item sunken-menu-item js-disable-pjax" data-hotkey="g w" data-selected-links="repo_wiki /magic3dcn/magic3d/wiki">
            <span class="octicon octicon-book"></span> <span class="full-word">Wiki</span>
            <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://assets-cdn.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>        </li>
    </ul>
    <div class="sunken-menu-separator"></div>
    <ul class="sunken-menu-group">

      <li class="tooltipped tooltipped-w" aria-label="Pulse">
        <a href="/magic3dcn/magic3d/pulse" aria-label="Pulse" class="js-selected-navigation-item sunken-menu-item" data-pjax="true" data-selected-links="pulse /magic3dcn/magic3d/pulse">
          <span class="octicon octicon-pulse"></span> <span class="full-word">Pulse</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://assets-cdn.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>

      <li class="tooltipped tooltipped-w" aria-label="Graphs">
        <a href="/magic3dcn/magic3d/graphs" aria-label="Graphs" class="js-selected-navigation-item sunken-menu-item" data-pjax="true" data-selected-links="repo_graphs repo_contributors /magic3dcn/magic3d/graphs">
          <span class="octicon octicon-graph"></span> <span class="full-word">Graphs</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://assets-cdn.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>

      <li class="tooltipped tooltipped-w" aria-label="Network">
        <a href="/magic3dcn/magic3d/network" aria-label="Network" class="js-selected-navigation-item sunken-menu-item js-disable-pjax" data-selected-links="repo_network /magic3dcn/magic3d/network">
          <span class="octicon octicon-git-branch"></span> <span class="full-word">Network</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://assets-cdn.github.com/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>
    </ul>


  </div>
</div>

              <div class="only-with-full-nav">
                

  

<div class="clone-url open"
  data-protocol-type="http"
  data-url="/users/set_protocol?protocol_selector=http&amp;protocol_type=push">
  <h3><strong>HTTPS</strong> clone URL</h3>
  <div class="clone-url-box">
    <input type="text" class="clone js-url-field"
           value="https://github.com/magic3dcn/magic3d.git" readonly="readonly">
    <span class="url-box-clippy">
    <button aria-label="copy to clipboard" class="js-zeroclipboard minibutton zeroclipboard-button" data-clipboard-text="https://github.com/magic3dcn/magic3d.git" data-copied-hint="copied!" type="button"><span class="octicon octicon-clippy"></span></button>
    </span>
  </div>
</div>

  

<div class="clone-url "
  data-protocol-type="ssh"
  data-url="/users/set_protocol?protocol_selector=ssh&amp;protocol_type=push">
  <h3><strong>SSH</strong> clone URL</h3>
  <div class="clone-url-box">
    <input type="text" class="clone js-url-field"
           value="git@github.com:magic3dcn/magic3d.git" readonly="readonly">
    <span class="url-box-clippy">
    <button aria-label="copy to clipboard" class="js-zeroclipboard minibutton zeroclipboard-button" data-clipboard-text="git@github.com:magic3dcn/magic3d.git" data-copied-hint="copied!" type="button"><span class="octicon octicon-clippy"></span></button>
    </span>
  </div>
</div>

  

<div class="clone-url "
  data-protocol-type="subversion"
  data-url="/users/set_protocol?protocol_selector=subversion&amp;protocol_type=push">
  <h3><strong>Subversion</strong> checkout URL</h3>
  <div class="clone-url-box">
    <input type="text" class="clone js-url-field"
           value="https://github.com/magic3dcn/magic3d" readonly="readonly">
    <span class="url-box-clippy">
    <button aria-label="copy to clipboard" class="js-zeroclipboard minibutton zeroclipboard-button" data-clipboard-text="https://github.com/magic3dcn/magic3d" data-copied-hint="copied!" type="button"><span class="octicon octicon-clippy"></span></button>
    </span>
  </div>
</div>


<p class="clone-options">You can clone with
      <a href="#" class="js-clone-selector" data-protocol="http">HTTPS</a>,
      <a href="#" class="js-clone-selector" data-protocol="ssh">SSH</a>,
      or <a href="#" class="js-clone-selector" data-protocol="subversion">Subversion</a>.
  <span class="help tooltipped tooltipped-n" aria-label="Get help on which URL is right for you.">
    <a href="https://help.github.com/articles/which-remote-url-should-i-use">
    <span class="octicon octicon-question"></span>
    </a>
  </span>
</p>

  <a href="http://mac.github.com" data-url="github-mac://openRepo/https://github.com/magic3dcn/magic3d" class="minibutton sidebar-button js-conduit-rewrite-url" title="Save magic3dcn/magic3d to your computer and use it in GitHub Desktop." aria-label="Save magic3dcn/magic3d to your computer and use it in GitHub Desktop.">
    <span class="octicon octicon-device-desktop"></span>
    Clone in Desktop
  </a>


                <a href="/magic3dcn/magic3d/archive/master.zip"
                   class="minibutton sidebar-button"
                   aria-label="Download magic3dcn/magic3d as a zip file"
                   title="Download magic3dcn/magic3d as a zip file"
                   rel="nofollow">
                  <span class="octicon octicon-cloud-download"></span>
                  Download ZIP
                </a>
              </div>
        </div><!-- /.repository-sidebar -->

        <div id="js-repo-pjax-container" class="repository-content context-loader-container" data-pjax-container>
          


<a href="/magic3dcn/magic3d/blob/f6481afa804745cdc4681bee1f541ebad661d8be/Src/AppModules/Face2D.cpp" class="hidden js-permalink-shortcut" data-hotkey="y">Permalink</a>

<!-- blob contrib key: blob_contributors:v21:fc5ace26a8d60664594e411ba8bb4d5e -->

<p title="This is a placeholder element" class="js-history-link-replace hidden"></p>

<a href="/magic3dcn/magic3d/find/master" data-pjax data-hotkey="t" class="js-show-file-finder" style="display:none">Show File Finder</a>

<div class="file-navigation">
  

<div class="select-menu js-menu-container js-select-menu" >
  <span class="minibutton select-menu-button js-menu-target" data-hotkey="w"
    data-master-branch="master"
    data-ref="master"
    role="button" aria-label="Switch branches or tags" tabindex="0" aria-haspopup="true">
    <span class="octicon octicon-git-branch"></span>
    <i>branch:</i>
    <span class="js-select-button">master</span>
  </span>

  <div class="select-menu-modal-holder js-menu-content js-navigation-container" data-pjax aria-hidden="true">

    <div class="select-menu-modal">
      <div class="select-menu-header">
        <span class="select-menu-title">Switch branches/tags</span>
        <span class="octicon octicon-remove-close js-menu-close"></span>
      </div> <!-- /.select-menu-header -->

      <div class="select-menu-filters">
        <div class="select-menu-text-filter">
          <input type="text" aria-label="Find or create a branch…" id="context-commitish-filter-field" class="js-filterable-field js-navigation-enable" placeholder="Find or create a branch…">
        </div>
        <div class="select-menu-tabs">
          <ul>
            <li class="select-menu-tab">
              <a href="#" data-tab-filter="branches" class="js-select-menu-tab">Branches</a>
            </li>
            <li class="select-menu-tab">
              <a href="#" data-tab-filter="tags" class="js-select-menu-tab">Tags</a>
            </li>
          </ul>
        </div><!-- /.select-menu-tabs -->
      </div><!-- /.select-menu-filters -->

      <div class="select-menu-list select-menu-tab-bucket js-select-menu-tab-bucket" data-tab-filter="branches">

        <div data-filterable-for="context-commitish-filter-field" data-filterable-type="substring">


            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/magic3dcn/magic3d/blob/develop/Src/AppModules/Face2D.cpp"
                 data-name="develop"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="develop">develop</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item selected">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/magic3dcn/magic3d/blob/master/Src/AppModules/Face2D.cpp"
                 data-name="master"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="master">master</a>
            </div> <!-- /.select-menu-item -->
        </div>

          <form accept-charset="UTF-8" action="/magic3dcn/magic3d/branches" class="js-create-branch select-menu-item select-menu-new-item-form js-navigation-item js-new-item-form" method="post"><div style="margin:0;padding:0;display:inline"><input name="authenticity_token" type="hidden" value="YOC5K+ltWpXcSYiEYjRkKeIMk/exigifckq9TBzRaR6hHZ5By4UqlDMXCzMrCloUOC3kWCEZTftrl89ZC3tJ1Q==" /></div>
            <span class="octicon octicon-git-branch-create select-menu-item-icon"></span>
            <div class="select-menu-item-text">
              <h4>Create branch: <span class="js-new-item-name"></span></h4>
              <span class="description">from ‘master’</span>
            </div>
            <input type="hidden" name="name" id="name" class="js-new-item-value">
            <input type="hidden" name="branch" id="branch" value="master" />
            <input type="hidden" name="path" id="path" value="Src/AppModules/Face2D.cpp" />
          </form> <!-- /.select-menu-item -->

      </div> <!-- /.select-menu-list -->

      <div class="select-menu-list select-menu-tab-bucket js-select-menu-tab-bucket" data-tab-filter="tags">
        <div data-filterable-for="context-commitish-filter-field" data-filterable-type="substring">


            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/magic3dcn/magic3d/tree/v0.4/Src/AppModules/Face2D.cpp"
                 data-name="v0.4"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="v0.4">v0.4</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/magic3dcn/magic3d/tree/V0.6.1/Src/AppModules/Face2D.cpp"
                 data-name="V0.6.1"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="V0.6.1">V0.6.1</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/magic3dcn/magic3d/tree/V0.5/Src/AppModules/Face2D.cpp"
                 data-name="V0.5"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="V0.5">V0.5</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/magic3dcn/magic3d/tree/ReliefFromMesh3D/Src/AppModules/Face2D.cpp"
                 data-name="ReliefFromMesh3D"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="ReliefFromMesh3D">ReliefFromMesh3D</a>
            </div> <!-- /.select-menu-item -->
        </div>

        <div class="select-menu-no-results">Nothing to show</div>
      </div> <!-- /.select-menu-list -->

    </div> <!-- /.select-menu-modal -->
  </div> <!-- /.select-menu-modal-holder -->
</div> <!-- /.select-menu -->

  <div class="breadcrumb">
    <span class='repo-root js-repo-root'><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/magic3dcn/magic3d" data-branch="master" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">magic3d</span></a></span></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/magic3dcn/magic3d/tree/master/Src" data-branch="master" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">Src</span></a></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/magic3dcn/magic3d/tree/master/Src/AppModules" data-branch="master" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">AppModules</span></a></span><span class="separator"> / </span><strong class="final-path">Face2D.cpp</strong> <button aria-label="copy to clipboard" class="js-zeroclipboard minibutton zeroclipboard-button" data-clipboard-text="Src/AppModules/Face2D.cpp" data-copied-hint="copied!" type="button"><span class="octicon octicon-clippy"></span></button>
  </div>
</div>


  <div class="commit commit-loader file-history-tease js-deferred-content" data-url="/magic3dcn/magic3d/contributors/master/Src/AppModules/Face2D.cpp">
    Fetching contributors…

    <div class="participation">
      <p class="loader-loading"><img alt="Octocat-spinner-32-eaf2f5" height="16" src="https://assets-cdn.github.com/images/spinners/octocat-spinner-32-EAF2F5.gif" width="16" /></p>
      <p class="loader-error">Cannot retrieve contributors at this time</p>
    </div>
  </div>

<div class="file-box">
  <div class="file">
    <div class="meta clearfix">
      <div class="info file-name">
        <span class="icon"><b class="octicon octicon-file-text"></b></span>
        <span class="mode" title="File Mode">file</span>
        <span class="meta-divider"></span>
          <span>620 lines (584 sloc)</span>
          <span class="meta-divider"></span>
        <span>19.193 kb</span>
      </div>
      <div class="actions">
        <div class="button-group">
            <a class="minibutton tooltipped tooltipped-w js-conduit-openfile-check"
               href="http://mac.github.com"
               data-url="github-mac://openRepo/https://github.com/magic3dcn/magic3d?branch=master&amp;filepath=Src%2FAppModules%2FFace2D.cpp"
               aria-label="Open this file in GitHub for Mac"
               data-failed-title="Your version of GitHub for Mac is too old to open this file. Try checking for updates.">
                <span class="octicon octicon-device-desktop"></span> Open
            </a>
                <a class="minibutton js-update-url-with-hash"
                   href="/magic3dcn/magic3d/edit/master/Src/AppModules/Face2D.cpp"
                   data-method="post" rel="nofollow" data-hotkey="e">Edit</a>
          <a href="/magic3dcn/magic3d/raw/master/Src/AppModules/Face2D.cpp" class="button minibutton " id="raw-url">Raw</a>
            <a href="/magic3dcn/magic3d/blame/master/Src/AppModules/Face2D.cpp" class="button minibutton js-update-url-with-hash">Blame</a>
          <a href="/magic3dcn/magic3d/commits/master/Src/AppModules/Face2D.cpp" class="button minibutton " rel="nofollow">History</a>
        </div><!-- /.button-group -->

            <a class="minibutton danger empty-icon"
               href="/magic3dcn/magic3d/delete/master/Src/AppModules/Face2D.cpp"
               data-method="post" data-test-id="delete-blob-file" rel="nofollow">

          Delete
        </a>
      </div><!-- /.actions -->
    </div>
        <div class="blob-wrapper data type-c js-blob-data">
        <table class="file-code file-diff tab-size-8">
          <tr class="file-code-line">
            <td class="blob-line-nums">
              <span id="L1" rel="#L1">1</span>
<span id="L2" rel="#L2">2</span>
<span id="L3" rel="#L3">3</span>
<span id="L4" rel="#L4">4</span>
<span id="L5" rel="#L5">5</span>
<span id="L6" rel="#L6">6</span>
<span id="L7" rel="#L7">7</span>
<span id="L8" rel="#L8">8</span>
<span id="L9" rel="#L9">9</span>
<span id="L10" rel="#L10">10</span>
<span id="L11" rel="#L11">11</span>
<span id="L12" rel="#L12">12</span>
<span id="L13" rel="#L13">13</span>
<span id="L14" rel="#L14">14</span>
<span id="L15" rel="#L15">15</span>
<span id="L16" rel="#L16">16</span>
<span id="L17" rel="#L17">17</span>
<span id="L18" rel="#L18">18</span>
<span id="L19" rel="#L19">19</span>
<span id="L20" rel="#L20">20</span>
<span id="L21" rel="#L21">21</span>
<span id="L22" rel="#L22">22</span>
<span id="L23" rel="#L23">23</span>
<span id="L24" rel="#L24">24</span>
<span id="L25" rel="#L25">25</span>
<span id="L26" rel="#L26">26</span>
<span id="L27" rel="#L27">27</span>
<span id="L28" rel="#L28">28</span>
<span id="L29" rel="#L29">29</span>
<span id="L30" rel="#L30">30</span>
<span id="L31" rel="#L31">31</span>
<span id="L32" rel="#L32">32</span>
<span id="L33" rel="#L33">33</span>
<span id="L34" rel="#L34">34</span>
<span id="L35" rel="#L35">35</span>
<span id="L36" rel="#L36">36</span>
<span id="L37" rel="#L37">37</span>
<span id="L38" rel="#L38">38</span>
<span id="L39" rel="#L39">39</span>
<span id="L40" rel="#L40">40</span>
<span id="L41" rel="#L41">41</span>
<span id="L42" rel="#L42">42</span>
<span id="L43" rel="#L43">43</span>
<span id="L44" rel="#L44">44</span>
<span id="L45" rel="#L45">45</span>
<span id="L46" rel="#L46">46</span>
<span id="L47" rel="#L47">47</span>
<span id="L48" rel="#L48">48</span>
<span id="L49" rel="#L49">49</span>
<span id="L50" rel="#L50">50</span>
<span id="L51" rel="#L51">51</span>
<span id="L52" rel="#L52">52</span>
<span id="L53" rel="#L53">53</span>
<span id="L54" rel="#L54">54</span>
<span id="L55" rel="#L55">55</span>
<span id="L56" rel="#L56">56</span>
<span id="L57" rel="#L57">57</span>
<span id="L58" rel="#L58">58</span>
<span id="L59" rel="#L59">59</span>
<span id="L60" rel="#L60">60</span>
<span id="L61" rel="#L61">61</span>
<span id="L62" rel="#L62">62</span>
<span id="L63" rel="#L63">63</span>
<span id="L64" rel="#L64">64</span>
<span id="L65" rel="#L65">65</span>
<span id="L66" rel="#L66">66</span>
<span id="L67" rel="#L67">67</span>
<span id="L68" rel="#L68">68</span>
<span id="L69" rel="#L69">69</span>
<span id="L70" rel="#L70">70</span>
<span id="L71" rel="#L71">71</span>
<span id="L72" rel="#L72">72</span>
<span id="L73" rel="#L73">73</span>
<span id="L74" rel="#L74">74</span>
<span id="L75" rel="#L75">75</span>
<span id="L76" rel="#L76">76</span>
<span id="L77" rel="#L77">77</span>
<span id="L78" rel="#L78">78</span>
<span id="L79" rel="#L79">79</span>
<span id="L80" rel="#L80">80</span>
<span id="L81" rel="#L81">81</span>
<span id="L82" rel="#L82">82</span>
<span id="L83" rel="#L83">83</span>
<span id="L84" rel="#L84">84</span>
<span id="L85" rel="#L85">85</span>
<span id="L86" rel="#L86">86</span>
<span id="L87" rel="#L87">87</span>
<span id="L88" rel="#L88">88</span>
<span id="L89" rel="#L89">89</span>
<span id="L90" rel="#L90">90</span>
<span id="L91" rel="#L91">91</span>
<span id="L92" rel="#L92">92</span>
<span id="L93" rel="#L93">93</span>
<span id="L94" rel="#L94">94</span>
<span id="L95" rel="#L95">95</span>
<span id="L96" rel="#L96">96</span>
<span id="L97" rel="#L97">97</span>
<span id="L98" rel="#L98">98</span>
<span id="L99" rel="#L99">99</span>
<span id="L100" rel="#L100">100</span>
<span id="L101" rel="#L101">101</span>
<span id="L102" rel="#L102">102</span>
<span id="L103" rel="#L103">103</span>
<span id="L104" rel="#L104">104</span>
<span id="L105" rel="#L105">105</span>
<span id="L106" rel="#L106">106</span>
<span id="L107" rel="#L107">107</span>
<span id="L108" rel="#L108">108</span>
<span id="L109" rel="#L109">109</span>
<span id="L110" rel="#L110">110</span>
<span id="L111" rel="#L111">111</span>
<span id="L112" rel="#L112">112</span>
<span id="L113" rel="#L113">113</span>
<span id="L114" rel="#L114">114</span>
<span id="L115" rel="#L115">115</span>
<span id="L116" rel="#L116">116</span>
<span id="L117" rel="#L117">117</span>
<span id="L118" rel="#L118">118</span>
<span id="L119" rel="#L119">119</span>
<span id="L120" rel="#L120">120</span>
<span id="L121" rel="#L121">121</span>
<span id="L122" rel="#L122">122</span>
<span id="L123" rel="#L123">123</span>
<span id="L124" rel="#L124">124</span>
<span id="L125" rel="#L125">125</span>
<span id="L126" rel="#L126">126</span>
<span id="L127" rel="#L127">127</span>
<span id="L128" rel="#L128">128</span>
<span id="L129" rel="#L129">129</span>
<span id="L130" rel="#L130">130</span>
<span id="L131" rel="#L131">131</span>
<span id="L132" rel="#L132">132</span>
<span id="L133" rel="#L133">133</span>
<span id="L134" rel="#L134">134</span>
<span id="L135" rel="#L135">135</span>
<span id="L136" rel="#L136">136</span>
<span id="L137" rel="#L137">137</span>
<span id="L138" rel="#L138">138</span>
<span id="L139" rel="#L139">139</span>
<span id="L140" rel="#L140">140</span>
<span id="L141" rel="#L141">141</span>
<span id="L142" rel="#L142">142</span>
<span id="L143" rel="#L143">143</span>
<span id="L144" rel="#L144">144</span>
<span id="L145" rel="#L145">145</span>
<span id="L146" rel="#L146">146</span>
<span id="L147" rel="#L147">147</span>
<span id="L148" rel="#L148">148</span>
<span id="L149" rel="#L149">149</span>
<span id="L150" rel="#L150">150</span>
<span id="L151" rel="#L151">151</span>
<span id="L152" rel="#L152">152</span>
<span id="L153" rel="#L153">153</span>
<span id="L154" rel="#L154">154</span>
<span id="L155" rel="#L155">155</span>
<span id="L156" rel="#L156">156</span>
<span id="L157" rel="#L157">157</span>
<span id="L158" rel="#L158">158</span>
<span id="L159" rel="#L159">159</span>
<span id="L160" rel="#L160">160</span>
<span id="L161" rel="#L161">161</span>
<span id="L162" rel="#L162">162</span>
<span id="L163" rel="#L163">163</span>
<span id="L164" rel="#L164">164</span>
<span id="L165" rel="#L165">165</span>
<span id="L166" rel="#L166">166</span>
<span id="L167" rel="#L167">167</span>
<span id="L168" rel="#L168">168</span>
<span id="L169" rel="#L169">169</span>
<span id="L170" rel="#L170">170</span>
<span id="L171" rel="#L171">171</span>
<span id="L172" rel="#L172">172</span>
<span id="L173" rel="#L173">173</span>
<span id="L174" rel="#L174">174</span>
<span id="L175" rel="#L175">175</span>
<span id="L176" rel="#L176">176</span>
<span id="L177" rel="#L177">177</span>
<span id="L178" rel="#L178">178</span>
<span id="L179" rel="#L179">179</span>
<span id="L180" rel="#L180">180</span>
<span id="L181" rel="#L181">181</span>
<span id="L182" rel="#L182">182</span>
<span id="L183" rel="#L183">183</span>
<span id="L184" rel="#L184">184</span>
<span id="L185" rel="#L185">185</span>
<span id="L186" rel="#L186">186</span>
<span id="L187" rel="#L187">187</span>
<span id="L188" rel="#L188">188</span>
<span id="L189" rel="#L189">189</span>
<span id="L190" rel="#L190">190</span>
<span id="L191" rel="#L191">191</span>
<span id="L192" rel="#L192">192</span>
<span id="L193" rel="#L193">193</span>
<span id="L194" rel="#L194">194</span>
<span id="L195" rel="#L195">195</span>
<span id="L196" rel="#L196">196</span>
<span id="L197" rel="#L197">197</span>
<span id="L198" rel="#L198">198</span>
<span id="L199" rel="#L199">199</span>
<span id="L200" rel="#L200">200</span>
<span id="L201" rel="#L201">201</span>
<span id="L202" rel="#L202">202</span>
<span id="L203" rel="#L203">203</span>
<span id="L204" rel="#L204">204</span>
<span id="L205" rel="#L205">205</span>
<span id="L206" rel="#L206">206</span>
<span id="L207" rel="#L207">207</span>
<span id="L208" rel="#L208">208</span>
<span id="L209" rel="#L209">209</span>
<span id="L210" rel="#L210">210</span>
<span id="L211" rel="#L211">211</span>
<span id="L212" rel="#L212">212</span>
<span id="L213" rel="#L213">213</span>
<span id="L214" rel="#L214">214</span>
<span id="L215" rel="#L215">215</span>
<span id="L216" rel="#L216">216</span>
<span id="L217" rel="#L217">217</span>
<span id="L218" rel="#L218">218</span>
<span id="L219" rel="#L219">219</span>
<span id="L220" rel="#L220">220</span>
<span id="L221" rel="#L221">221</span>
<span id="L222" rel="#L222">222</span>
<span id="L223" rel="#L223">223</span>
<span id="L224" rel="#L224">224</span>
<span id="L225" rel="#L225">225</span>
<span id="L226" rel="#L226">226</span>
<span id="L227" rel="#L227">227</span>
<span id="L228" rel="#L228">228</span>
<span id="L229" rel="#L229">229</span>
<span id="L230" rel="#L230">230</span>
<span id="L231" rel="#L231">231</span>
<span id="L232" rel="#L232">232</span>
<span id="L233" rel="#L233">233</span>
<span id="L234" rel="#L234">234</span>
<span id="L235" rel="#L235">235</span>
<span id="L236" rel="#L236">236</span>
<span id="L237" rel="#L237">237</span>
<span id="L238" rel="#L238">238</span>
<span id="L239" rel="#L239">239</span>
<span id="L240" rel="#L240">240</span>
<span id="L241" rel="#L241">241</span>
<span id="L242" rel="#L242">242</span>
<span id="L243" rel="#L243">243</span>
<span id="L244" rel="#L244">244</span>
<span id="L245" rel="#L245">245</span>
<span id="L246" rel="#L246">246</span>
<span id="L247" rel="#L247">247</span>
<span id="L248" rel="#L248">248</span>
<span id="L249" rel="#L249">249</span>
<span id="L250" rel="#L250">250</span>
<span id="L251" rel="#L251">251</span>
<span id="L252" rel="#L252">252</span>
<span id="L253" rel="#L253">253</span>
<span id="L254" rel="#L254">254</span>
<span id="L255" rel="#L255">255</span>
<span id="L256" rel="#L256">256</span>
<span id="L257" rel="#L257">257</span>
<span id="L258" rel="#L258">258</span>
<span id="L259" rel="#L259">259</span>
<span id="L260" rel="#L260">260</span>
<span id="L261" rel="#L261">261</span>
<span id="L262" rel="#L262">262</span>
<span id="L263" rel="#L263">263</span>
<span id="L264" rel="#L264">264</span>
<span id="L265" rel="#L265">265</span>
<span id="L266" rel="#L266">266</span>
<span id="L267" rel="#L267">267</span>
<span id="L268" rel="#L268">268</span>
<span id="L269" rel="#L269">269</span>
<span id="L270" rel="#L270">270</span>
<span id="L271" rel="#L271">271</span>
<span id="L272" rel="#L272">272</span>
<span id="L273" rel="#L273">273</span>
<span id="L274" rel="#L274">274</span>
<span id="L275" rel="#L275">275</span>
<span id="L276" rel="#L276">276</span>
<span id="L277" rel="#L277">277</span>
<span id="L278" rel="#L278">278</span>
<span id="L279" rel="#L279">279</span>
<span id="L280" rel="#L280">280</span>
<span id="L281" rel="#L281">281</span>
<span id="L282" rel="#L282">282</span>
<span id="L283" rel="#L283">283</span>
<span id="L284" rel="#L284">284</span>
<span id="L285" rel="#L285">285</span>
<span id="L286" rel="#L286">286</span>
<span id="L287" rel="#L287">287</span>
<span id="L288" rel="#L288">288</span>
<span id="L289" rel="#L289">289</span>
<span id="L290" rel="#L290">290</span>
<span id="L291" rel="#L291">291</span>
<span id="L292" rel="#L292">292</span>
<span id="L293" rel="#L293">293</span>
<span id="L294" rel="#L294">294</span>
<span id="L295" rel="#L295">295</span>
<span id="L296" rel="#L296">296</span>
<span id="L297" rel="#L297">297</span>
<span id="L298" rel="#L298">298</span>
<span id="L299" rel="#L299">299</span>
<span id="L300" rel="#L300">300</span>
<span id="L301" rel="#L301">301</span>
<span id="L302" rel="#L302">302</span>
<span id="L303" rel="#L303">303</span>
<span id="L304" rel="#L304">304</span>
<span id="L305" rel="#L305">305</span>
<span id="L306" rel="#L306">306</span>
<span id="L307" rel="#L307">307</span>
<span id="L308" rel="#L308">308</span>
<span id="L309" rel="#L309">309</span>
<span id="L310" rel="#L310">310</span>
<span id="L311" rel="#L311">311</span>
<span id="L312" rel="#L312">312</span>
<span id="L313" rel="#L313">313</span>
<span id="L314" rel="#L314">314</span>
<span id="L315" rel="#L315">315</span>
<span id="L316" rel="#L316">316</span>
<span id="L317" rel="#L317">317</span>
<span id="L318" rel="#L318">318</span>
<span id="L319" rel="#L319">319</span>
<span id="L320" rel="#L320">320</span>
<span id="L321" rel="#L321">321</span>
<span id="L322" rel="#L322">322</span>
<span id="L323" rel="#L323">323</span>
<span id="L324" rel="#L324">324</span>
<span id="L325" rel="#L325">325</span>
<span id="L326" rel="#L326">326</span>
<span id="L327" rel="#L327">327</span>
<span id="L328" rel="#L328">328</span>
<span id="L329" rel="#L329">329</span>
<span id="L330" rel="#L330">330</span>
<span id="L331" rel="#L331">331</span>
<span id="L332" rel="#L332">332</span>
<span id="L333" rel="#L333">333</span>
<span id="L334" rel="#L334">334</span>
<span id="L335" rel="#L335">335</span>
<span id="L336" rel="#L336">336</span>
<span id="L337" rel="#L337">337</span>
<span id="L338" rel="#L338">338</span>
<span id="L339" rel="#L339">339</span>
<span id="L340" rel="#L340">340</span>
<span id="L341" rel="#L341">341</span>
<span id="L342" rel="#L342">342</span>
<span id="L343" rel="#L343">343</span>
<span id="L344" rel="#L344">344</span>
<span id="L345" rel="#L345">345</span>
<span id="L346" rel="#L346">346</span>
<span id="L347" rel="#L347">347</span>
<span id="L348" rel="#L348">348</span>
<span id="L349" rel="#L349">349</span>
<span id="L350" rel="#L350">350</span>
<span id="L351" rel="#L351">351</span>
<span id="L352" rel="#L352">352</span>
<span id="L353" rel="#L353">353</span>
<span id="L354" rel="#L354">354</span>
<span id="L355" rel="#L355">355</span>
<span id="L356" rel="#L356">356</span>
<span id="L357" rel="#L357">357</span>
<span id="L358" rel="#L358">358</span>
<span id="L359" rel="#L359">359</span>
<span id="L360" rel="#L360">360</span>
<span id="L361" rel="#L361">361</span>
<span id="L362" rel="#L362">362</span>
<span id="L363" rel="#L363">363</span>
<span id="L364" rel="#L364">364</span>
<span id="L365" rel="#L365">365</span>
<span id="L366" rel="#L366">366</span>
<span id="L367" rel="#L367">367</span>
<span id="L368" rel="#L368">368</span>
<span id="L369" rel="#L369">369</span>
<span id="L370" rel="#L370">370</span>
<span id="L371" rel="#L371">371</span>
<span id="L372" rel="#L372">372</span>
<span id="L373" rel="#L373">373</span>
<span id="L374" rel="#L374">374</span>
<span id="L375" rel="#L375">375</span>
<span id="L376" rel="#L376">376</span>
<span id="L377" rel="#L377">377</span>
<span id="L378" rel="#L378">378</span>
<span id="L379" rel="#L379">379</span>
<span id="L380" rel="#L380">380</span>
<span id="L381" rel="#L381">381</span>
<span id="L382" rel="#L382">382</span>
<span id="L383" rel="#L383">383</span>
<span id="L384" rel="#L384">384</span>
<span id="L385" rel="#L385">385</span>
<span id="L386" rel="#L386">386</span>
<span id="L387" rel="#L387">387</span>
<span id="L388" rel="#L388">388</span>
<span id="L389" rel="#L389">389</span>
<span id="L390" rel="#L390">390</span>
<span id="L391" rel="#L391">391</span>
<span id="L392" rel="#L392">392</span>
<span id="L393" rel="#L393">393</span>
<span id="L394" rel="#L394">394</span>
<span id="L395" rel="#L395">395</span>
<span id="L396" rel="#L396">396</span>
<span id="L397" rel="#L397">397</span>
<span id="L398" rel="#L398">398</span>
<span id="L399" rel="#L399">399</span>
<span id="L400" rel="#L400">400</span>
<span id="L401" rel="#L401">401</span>
<span id="L402" rel="#L402">402</span>
<span id="L403" rel="#L403">403</span>
<span id="L404" rel="#L404">404</span>
<span id="L405" rel="#L405">405</span>
<span id="L406" rel="#L406">406</span>
<span id="L407" rel="#L407">407</span>
<span id="L408" rel="#L408">408</span>
<span id="L409" rel="#L409">409</span>
<span id="L410" rel="#L410">410</span>
<span id="L411" rel="#L411">411</span>
<span id="L412" rel="#L412">412</span>
<span id="L413" rel="#L413">413</span>
<span id="L414" rel="#L414">414</span>
<span id="L415" rel="#L415">415</span>
<span id="L416" rel="#L416">416</span>
<span id="L417" rel="#L417">417</span>
<span id="L418" rel="#L418">418</span>
<span id="L419" rel="#L419">419</span>
<span id="L420" rel="#L420">420</span>
<span id="L421" rel="#L421">421</span>
<span id="L422" rel="#L422">422</span>
<span id="L423" rel="#L423">423</span>
<span id="L424" rel="#L424">424</span>
<span id="L425" rel="#L425">425</span>
<span id="L426" rel="#L426">426</span>
<span id="L427" rel="#L427">427</span>
<span id="L428" rel="#L428">428</span>
<span id="L429" rel="#L429">429</span>
<span id="L430" rel="#L430">430</span>
<span id="L431" rel="#L431">431</span>
<span id="L432" rel="#L432">432</span>
<span id="L433" rel="#L433">433</span>
<span id="L434" rel="#L434">434</span>
<span id="L435" rel="#L435">435</span>
<span id="L436" rel="#L436">436</span>
<span id="L437" rel="#L437">437</span>
<span id="L438" rel="#L438">438</span>
<span id="L439" rel="#L439">439</span>
<span id="L440" rel="#L440">440</span>
<span id="L441" rel="#L441">441</span>
<span id="L442" rel="#L442">442</span>
<span id="L443" rel="#L443">443</span>
<span id="L444" rel="#L444">444</span>
<span id="L445" rel="#L445">445</span>
<span id="L446" rel="#L446">446</span>
<span id="L447" rel="#L447">447</span>
<span id="L448" rel="#L448">448</span>
<span id="L449" rel="#L449">449</span>
<span id="L450" rel="#L450">450</span>
<span id="L451" rel="#L451">451</span>
<span id="L452" rel="#L452">452</span>
<span id="L453" rel="#L453">453</span>
<span id="L454" rel="#L454">454</span>
<span id="L455" rel="#L455">455</span>
<span id="L456" rel="#L456">456</span>
<span id="L457" rel="#L457">457</span>
<span id="L458" rel="#L458">458</span>
<span id="L459" rel="#L459">459</span>
<span id="L460" rel="#L460">460</span>
<span id="L461" rel="#L461">461</span>
<span id="L462" rel="#L462">462</span>
<span id="L463" rel="#L463">463</span>
<span id="L464" rel="#L464">464</span>
<span id="L465" rel="#L465">465</span>
<span id="L466" rel="#L466">466</span>
<span id="L467" rel="#L467">467</span>
<span id="L468" rel="#L468">468</span>
<span id="L469" rel="#L469">469</span>
<span id="L470" rel="#L470">470</span>
<span id="L471" rel="#L471">471</span>
<span id="L472" rel="#L472">472</span>
<span id="L473" rel="#L473">473</span>
<span id="L474" rel="#L474">474</span>
<span id="L475" rel="#L475">475</span>
<span id="L476" rel="#L476">476</span>
<span id="L477" rel="#L477">477</span>
<span id="L478" rel="#L478">478</span>
<span id="L479" rel="#L479">479</span>
<span id="L480" rel="#L480">480</span>
<span id="L481" rel="#L481">481</span>
<span id="L482" rel="#L482">482</span>
<span id="L483" rel="#L483">483</span>
<span id="L484" rel="#L484">484</span>
<span id="L485" rel="#L485">485</span>
<span id="L486" rel="#L486">486</span>
<span id="L487" rel="#L487">487</span>
<span id="L488" rel="#L488">488</span>
<span id="L489" rel="#L489">489</span>
<span id="L490" rel="#L490">490</span>
<span id="L491" rel="#L491">491</span>
<span id="L492" rel="#L492">492</span>
<span id="L493" rel="#L493">493</span>
<span id="L494" rel="#L494">494</span>
<span id="L495" rel="#L495">495</span>
<span id="L496" rel="#L496">496</span>
<span id="L497" rel="#L497">497</span>
<span id="L498" rel="#L498">498</span>
<span id="L499" rel="#L499">499</span>
<span id="L500" rel="#L500">500</span>
<span id="L501" rel="#L501">501</span>
<span id="L502" rel="#L502">502</span>
<span id="L503" rel="#L503">503</span>
<span id="L504" rel="#L504">504</span>
<span id="L505" rel="#L505">505</span>
<span id="L506" rel="#L506">506</span>
<span id="L507" rel="#L507">507</span>
<span id="L508" rel="#L508">508</span>
<span id="L509" rel="#L509">509</span>
<span id="L510" rel="#L510">510</span>
<span id="L511" rel="#L511">511</span>
<span id="L512" rel="#L512">512</span>
<span id="L513" rel="#L513">513</span>
<span id="L514" rel="#L514">514</span>
<span id="L515" rel="#L515">515</span>
<span id="L516" rel="#L516">516</span>
<span id="L517" rel="#L517">517</span>
<span id="L518" rel="#L518">518</span>
<span id="L519" rel="#L519">519</span>
<span id="L520" rel="#L520">520</span>
<span id="L521" rel="#L521">521</span>
<span id="L522" rel="#L522">522</span>
<span id="L523" rel="#L523">523</span>
<span id="L524" rel="#L524">524</span>
<span id="L525" rel="#L525">525</span>
<span id="L526" rel="#L526">526</span>
<span id="L527" rel="#L527">527</span>
<span id="L528" rel="#L528">528</span>
<span id="L529" rel="#L529">529</span>
<span id="L530" rel="#L530">530</span>
<span id="L531" rel="#L531">531</span>
<span id="L532" rel="#L532">532</span>
<span id="L533" rel="#L533">533</span>
<span id="L534" rel="#L534">534</span>
<span id="L535" rel="#L535">535</span>
<span id="L536" rel="#L536">536</span>
<span id="L537" rel="#L537">537</span>
<span id="L538" rel="#L538">538</span>
<span id="L539" rel="#L539">539</span>
<span id="L540" rel="#L540">540</span>
<span id="L541" rel="#L541">541</span>
<span id="L542" rel="#L542">542</span>
<span id="L543" rel="#L543">543</span>
<span id="L544" rel="#L544">544</span>
<span id="L545" rel="#L545">545</span>
<span id="L546" rel="#L546">546</span>
<span id="L547" rel="#L547">547</span>
<span id="L548" rel="#L548">548</span>
<span id="L549" rel="#L549">549</span>
<span id="L550" rel="#L550">550</span>
<span id="L551" rel="#L551">551</span>
<span id="L552" rel="#L552">552</span>
<span id="L553" rel="#L553">553</span>
<span id="L554" rel="#L554">554</span>
<span id="L555" rel="#L555">555</span>
<span id="L556" rel="#L556">556</span>
<span id="L557" rel="#L557">557</span>
<span id="L558" rel="#L558">558</span>
<span id="L559" rel="#L559">559</span>
<span id="L560" rel="#L560">560</span>
<span id="L561" rel="#L561">561</span>
<span id="L562" rel="#L562">562</span>
<span id="L563" rel="#L563">563</span>
<span id="L564" rel="#L564">564</span>
<span id="L565" rel="#L565">565</span>
<span id="L566" rel="#L566">566</span>
<span id="L567" rel="#L567">567</span>
<span id="L568" rel="#L568">568</span>
<span id="L569" rel="#L569">569</span>
<span id="L570" rel="#L570">570</span>
<span id="L571" rel="#L571">571</span>
<span id="L572" rel="#L572">572</span>
<span id="L573" rel="#L573">573</span>
<span id="L574" rel="#L574">574</span>
<span id="L575" rel="#L575">575</span>
<span id="L576" rel="#L576">576</span>
<span id="L577" rel="#L577">577</span>
<span id="L578" rel="#L578">578</span>
<span id="L579" rel="#L579">579</span>
<span id="L580" rel="#L580">580</span>
<span id="L581" rel="#L581">581</span>
<span id="L582" rel="#L582">582</span>
<span id="L583" rel="#L583">583</span>
<span id="L584" rel="#L584">584</span>
<span id="L585" rel="#L585">585</span>
<span id="L586" rel="#L586">586</span>
<span id="L587" rel="#L587">587</span>
<span id="L588" rel="#L588">588</span>
<span id="L589" rel="#L589">589</span>
<span id="L590" rel="#L590">590</span>
<span id="L591" rel="#L591">591</span>
<span id="L592" rel="#L592">592</span>
<span id="L593" rel="#L593">593</span>
<span id="L594" rel="#L594">594</span>
<span id="L595" rel="#L595">595</span>
<span id="L596" rel="#L596">596</span>
<span id="L597" rel="#L597">597</span>
<span id="L598" rel="#L598">598</span>
<span id="L599" rel="#L599">599</span>
<span id="L600" rel="#L600">600</span>
<span id="L601" rel="#L601">601</span>
<span id="L602" rel="#L602">602</span>
<span id="L603" rel="#L603">603</span>
<span id="L604" rel="#L604">604</span>
<span id="L605" rel="#L605">605</span>
<span id="L606" rel="#L606">606</span>
<span id="L607" rel="#L607">607</span>
<span id="L608" rel="#L608">608</span>
<span id="L609" rel="#L609">609</span>
<span id="L610" rel="#L610">610</span>
<span id="L611" rel="#L611">611</span>
<span id="L612" rel="#L612">612</span>
<span id="L613" rel="#L613">613</span>
<span id="L614" rel="#L614">614</span>
<span id="L615" rel="#L615">615</span>
<span id="L616" rel="#L616">616</span>
<span id="L617" rel="#L617">617</span>
<span id="L618" rel="#L618">618</span>
<span id="L619" rel="#L619">619</span>

            </td>
            <td class="blob-line-code"><div class="code-body highlight"><pre><div class='line' id='LC1'><span class="cp">#include &quot;Face2D.h&quot;</span></div><div class='line' id='LC2'><span class="cp">#include &lt;fstream&gt;</span></div><div class='line' id='LC3'><br/></div><div class='line' id='LC4'><span class="k">namespace</span> <span class="n">MagicApp</span></div><div class='line' id='LC5'><span class="p">{</span></div><div class='line' id='LC6'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">FaceFeaturePoint</span><span class="o">::</span><span class="n">FaceFeaturePoint</span><span class="p">()</span> <span class="o">:</span></div><div class='line' id='LC7'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectIndex</span><span class="p">(</span><span class="o">-</span><span class="mi">1</span><span class="p">),</span></div><div class='line' id='LC8'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectType</span><span class="p">(</span><span class="n">FT_None</span><span class="p">)</span></div><div class='line' id='LC9'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC10'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC11'><br/></div><div class='line' id='LC12'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">FaceFeaturePoint</span><span class="o">::~</span><span class="n">FaceFeaturePoint</span><span class="p">()</span></div><div class='line' id='LC13'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC14'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC15'><br/></div><div class='line' id='LC16'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="cm">/*void FaceFeaturePoint::Load()</span></div><div class='line' id='LC17'><span class="cm">    {</span></div><div class='line' id='LC18'><span class="cm">        std::string fileName;</span></div><div class='line' id='LC19'><span class="cm">        char filterName[] = &quot;Feature Point Files(*.fp)\0*.fp\0&quot;;</span></div><div class='line' id='LC20'><span class="cm">        if (MagicCore::ToolKit::FileOpenDlg(fileName, filterName))</span></div><div class='line' id='LC21'><span class="cm">        {</span></div><div class='line' id='LC22'><span class="cm">            Load(fileName);</span></div><div class='line' id='LC23'><span class="cm">        }</span></div><div class='line' id='LC24'><span class="cm">    }*/</span></div><div class='line' id='LC25'><br/></div><div class='line' id='LC26'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">bool</span> <span class="n">FaceFeaturePoint</span><span class="o">::</span><span class="n">Load</span><span class="p">(</span><span class="k">const</span> <span class="n">std</span><span class="o">::</span><span class="n">string</span><span class="o">&amp;</span> <span class="n">fileName</span><span class="p">)</span></div><div class='line' id='LC27'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC28'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">std</span><span class="o">::</span><span class="n">ifstream</span> <span class="n">fin</span><span class="p">(</span><span class="n">fileName</span><span class="p">);</span></div><div class='line' id='LC29'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="o">!</span><span class="n">fin</span><span class="p">.</span><span class="n">is_open</span><span class="p">())</span></div><div class='line' id='LC30'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC31'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="nb">false</span><span class="p">;</span></div><div class='line' id='LC32'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC33'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">leftBrowNum</span><span class="p">;</span></div><div class='line' id='LC34'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fin</span> <span class="o">&gt;&gt;</span> <span class="n">leftBrowNum</span><span class="p">;</span></div><div class='line' id='LC35'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC36'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">resize</span><span class="p">(</span><span class="n">leftBrowNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC37'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">leftBrowNum</span><span class="p">;</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC38'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC39'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">hid</span><span class="p">,</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC40'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fin</span> <span class="o">&gt;&gt;</span> <span class="n">hid</span> <span class="o">&gt;&gt;</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC41'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">hid</span><span class="p">;</span></div><div class='line' id='LC42'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC43'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC44'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">rightBrowNum</span><span class="p">;</span></div><div class='line' id='LC45'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fin</span> <span class="o">&gt;&gt;</span> <span class="n">rightBrowNum</span><span class="p">;</span></div><div class='line' id='LC46'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC47'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">resize</span><span class="p">(</span><span class="n">rightBrowNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC48'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">rightBrowNum</span><span class="p">;</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC49'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC50'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">hid</span><span class="p">,</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC51'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fin</span> <span class="o">&gt;&gt;</span> <span class="n">hid</span> <span class="o">&gt;&gt;</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC52'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">hid</span><span class="p">;</span></div><div class='line' id='LC53'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC54'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC55'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">leftEyeNum</span><span class="p">;</span></div><div class='line' id='LC56'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fin</span> <span class="o">&gt;&gt;</span> <span class="n">leftEyeNum</span><span class="p">;</span></div><div class='line' id='LC57'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC58'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">resize</span><span class="p">(</span><span class="n">leftEyeNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC59'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">leftEyeNum</span><span class="p">;</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC60'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC61'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">hid</span><span class="p">,</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC62'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fin</span> <span class="o">&gt;&gt;</span> <span class="n">hid</span> <span class="o">&gt;&gt;</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC63'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">hid</span><span class="p">;</span></div><div class='line' id='LC64'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC65'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC66'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">rightEyeNum</span><span class="p">;</span></div><div class='line' id='LC67'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fin</span> <span class="o">&gt;&gt;</span> <span class="n">rightEyeNum</span><span class="p">;</span></div><div class='line' id='LC68'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC69'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">resize</span><span class="p">(</span><span class="n">rightEyeNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC70'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">rightEyeNum</span><span class="p">;</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC71'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC72'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">hid</span><span class="p">,</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC73'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fin</span> <span class="o">&gt;&gt;</span> <span class="n">hid</span> <span class="o">&gt;&gt;</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC74'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">hid</span><span class="p">;</span></div><div class='line' id='LC75'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC76'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC77'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">noseNum</span><span class="p">;</span></div><div class='line' id='LC78'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fin</span> <span class="o">&gt;&gt;</span> <span class="n">noseNum</span><span class="p">;</span></div><div class='line' id='LC79'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mNoseFPs</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC80'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mNoseFPs</span><span class="p">.</span><span class="n">resize</span><span class="p">(</span><span class="n">noseNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC81'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">noseNum</span><span class="p">;</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC82'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC83'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">hid</span><span class="p">,</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC84'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fin</span> <span class="o">&gt;&gt;</span> <span class="n">hid</span> <span class="o">&gt;&gt;</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC85'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mNoseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">hid</span><span class="p">;</span></div><div class='line' id='LC86'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mNoseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC87'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC88'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">mouseNum</span><span class="p">;</span></div><div class='line' id='LC89'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fin</span> <span class="o">&gt;&gt;</span> <span class="n">mouseNum</span><span class="p">;</span></div><div class='line' id='LC90'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mMouseFPs</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC91'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mMouseFPs</span><span class="p">.</span><span class="n">resize</span><span class="p">(</span><span class="n">mouseNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC92'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">mouseNum</span><span class="p">;</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC93'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC94'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">hid</span><span class="p">,</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC95'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fin</span> <span class="o">&gt;&gt;</span> <span class="n">hid</span> <span class="o">&gt;&gt;</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC96'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mMouseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">hid</span><span class="p">;</span></div><div class='line' id='LC97'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mMouseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC98'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC99'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">borderNum</span><span class="p">;</span></div><div class='line' id='LC100'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fin</span> <span class="o">&gt;&gt;</span> <span class="n">borderNum</span><span class="p">;</span></div><div class='line' id='LC101'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mBorderFPs</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC102'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mBorderFPs</span><span class="p">.</span><span class="n">resize</span><span class="p">(</span><span class="n">borderNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC103'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">borderNum</span><span class="p">;</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC104'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC105'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">hid</span><span class="p">,</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC106'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fin</span> <span class="o">&gt;&gt;</span> <span class="n">hid</span> <span class="o">&gt;&gt;</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC107'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mBorderFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">hid</span><span class="p">;</span></div><div class='line' id='LC108'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mBorderFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC109'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC110'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fin</span><span class="p">.</span><span class="n">close</span><span class="p">();</span></div><div class='line' id='LC111'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">UpdateDPs</span><span class="p">();</span></div><div class='line' id='LC112'><br/></div><div class='line' id='LC113'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="nb">true</span><span class="p">;</span></div><div class='line' id='LC114'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC115'><br/></div><div class='line' id='LC116'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">void</span> <span class="n">FaceFeaturePoint</span><span class="o">::</span><span class="n">Load</span><span class="p">(</span><span class="kt">int</span> <span class="n">browNum</span><span class="p">,</span> <span class="kt">int</span> <span class="n">eyeNum</span><span class="p">,</span> <span class="kt">int</span> <span class="n">noseNum</span><span class="p">,</span> <span class="kt">int</span> <span class="n">mouseNum</span><span class="p">,</span> <span class="kt">int</span> <span class="n">borderNum</span><span class="p">,</span> <span class="k">const</span> <span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;&amp;</span> <span class="n">posList</span><span class="p">)</span></div><div class='line' id='LC117'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC118'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">baseIndex</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span></div><div class='line' id='LC119'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC120'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">resize</span><span class="p">(</span><span class="n">browNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC121'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">pointId</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">pointId</span> <span class="o">&lt;</span> <span class="n">browNum</span><span class="p">;</span> <span class="n">pointId</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC122'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC123'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">posList</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">baseIndex</span> <span class="o">+</span> <span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC124'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">posList</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">baseIndex</span> <span class="o">+</span> <span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC125'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC126'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">baseIndex</span> <span class="o">+=</span> <span class="p">(</span><span class="n">browNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC127'><br/></div><div class='line' id='LC128'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC129'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">resize</span><span class="p">(</span><span class="n">browNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC130'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">pointId</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">pointId</span> <span class="o">&lt;</span> <span class="n">browNum</span><span class="p">;</span> <span class="n">pointId</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC131'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC132'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">posList</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">baseIndex</span> <span class="o">+</span> <span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC133'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">posList</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">baseIndex</span> <span class="o">+</span> <span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC134'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC135'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">baseIndex</span> <span class="o">+=</span> <span class="p">(</span><span class="n">browNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC136'><br/></div><div class='line' id='LC137'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC138'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">resize</span><span class="p">(</span><span class="n">eyeNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC139'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">pointId</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">pointId</span> <span class="o">&lt;</span> <span class="n">eyeNum</span><span class="p">;</span> <span class="n">pointId</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC140'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC141'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">posList</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">baseIndex</span> <span class="o">+</span> <span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC142'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">posList</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">baseIndex</span> <span class="o">+</span> <span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC143'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC144'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">baseIndex</span> <span class="o">+=</span> <span class="p">(</span><span class="n">eyeNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC145'><br/></div><div class='line' id='LC146'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC147'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">resize</span><span class="p">(</span><span class="n">eyeNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC148'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">pointId</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">pointId</span> <span class="o">&lt;</span> <span class="n">eyeNum</span><span class="p">;</span> <span class="n">pointId</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC149'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC150'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">posList</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">baseIndex</span> <span class="o">+</span> <span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC151'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">posList</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">baseIndex</span> <span class="o">+</span> <span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC152'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC153'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">baseIndex</span> <span class="o">+=</span> <span class="p">(</span><span class="n">eyeNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC154'><br/></div><div class='line' id='LC155'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mNoseFPs</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC156'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mNoseFPs</span><span class="p">.</span><span class="n">resize</span><span class="p">(</span><span class="n">noseNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC157'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">pointId</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">pointId</span> <span class="o">&lt;</span> <span class="n">noseNum</span><span class="p">;</span> <span class="n">pointId</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC158'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC159'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mNoseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">posList</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">baseIndex</span> <span class="o">+</span> <span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC160'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mNoseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">posList</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">baseIndex</span> <span class="o">+</span> <span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC161'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC162'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">baseIndex</span> <span class="o">+=</span> <span class="p">(</span><span class="n">noseNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC163'><br/></div><div class='line' id='LC164'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mMouseFPs</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC165'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mMouseFPs</span><span class="p">.</span><span class="n">resize</span><span class="p">(</span><span class="n">mouseNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC166'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">pointId</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">pointId</span> <span class="o">&lt;</span> <span class="n">mouseNum</span><span class="p">;</span> <span class="n">pointId</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC167'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC168'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mMouseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">posList</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">baseIndex</span> <span class="o">+</span> <span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC169'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mMouseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">posList</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">baseIndex</span> <span class="o">+</span> <span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC170'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC171'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">baseIndex</span> <span class="o">+=</span> <span class="p">(</span><span class="n">mouseNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC172'><br/></div><div class='line' id='LC173'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mBorderFPs</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC174'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mBorderFPs</span><span class="p">.</span><span class="n">resize</span><span class="p">(</span><span class="n">borderNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC175'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">pointId</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">pointId</span> <span class="o">&lt;</span> <span class="n">borderNum</span><span class="p">;</span> <span class="n">pointId</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC176'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC177'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mBorderFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">posList</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">baseIndex</span> <span class="o">+</span> <span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC178'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mBorderFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">posList</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">baseIndex</span> <span class="o">+</span> <span class="n">pointId</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC179'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC180'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">baseIndex</span> <span class="o">+=</span> <span class="p">(</span><span class="n">borderNum</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC181'><br/></div><div class='line' id='LC182'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">UpdateDPs</span><span class="p">();</span></div><div class='line' id='LC183'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC184'><br/></div><div class='line' id='LC185'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">void</span> <span class="n">FaceFeaturePoint</span><span class="o">::</span><span class="n">UpdateDPs</span><span class="p">()</span></div><div class='line' id='LC186'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC187'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">ConstructOneDPs</span><span class="p">(</span><span class="n">mLeftBrowFPs</span><span class="p">,</span> <span class="nb">true</span><span class="p">,</span> <span class="mi">2</span><span class="p">,</span> <span class="n">mLeftBrowDPs</span><span class="p">);</span></div><div class='line' id='LC188'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">ConstructOneDPs</span><span class="p">(</span><span class="n">mRightBrowFPs</span><span class="p">,</span> <span class="nb">true</span><span class="p">,</span> <span class="mi">2</span><span class="p">,</span> <span class="n">mRightBrowDPs</span><span class="p">);</span></div><div class='line' id='LC189'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">ConstructOneDPs</span><span class="p">(</span><span class="n">mLeftEyeFPs</span><span class="p">,</span> <span class="nb">true</span><span class="p">,</span> <span class="mi">2</span><span class="p">,</span> <span class="n">mLeftEyeDPs</span><span class="p">);</span></div><div class='line' id='LC190'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">ConstructOneDPs</span><span class="p">(</span><span class="n">mRightEyeFPs</span><span class="p">,</span> <span class="nb">true</span><span class="p">,</span> <span class="mi">2</span><span class="p">,</span> <span class="n">mRightEyeDPs</span><span class="p">);</span></div><div class='line' id='LC191'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">ConstructOneDPs</span><span class="p">(</span><span class="n">mNoseFPs</span><span class="p">,</span> <span class="nb">false</span><span class="p">,</span> <span class="mi">2</span><span class="p">,</span> <span class="n">mNoseDPs</span><span class="p">);</span></div><div class='line' id='LC192'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">ConstructOneDPs</span><span class="p">(</span><span class="n">mMouseFPs</span><span class="p">,</span> <span class="nb">true</span><span class="p">,</span> <span class="mi">2</span><span class="p">,</span> <span class="n">mMouseDPs</span><span class="p">);</span></div><div class='line' id='LC193'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">ConstructOneDPs</span><span class="p">(</span><span class="n">mBorderFPs</span><span class="p">,</span> <span class="nb">false</span><span class="p">,</span> <span class="mi">10</span><span class="p">,</span> <span class="n">mBorderDPs</span><span class="p">);</span></div><div class='line' id='LC194'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC195'><br/></div><div class='line' id='LC196'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">void</span> <span class="n">FaceFeaturePoint</span><span class="o">::</span><span class="n">ConstructOneDPs</span><span class="p">(</span><span class="k">const</span> <span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;&amp;</span> <span class="n">fps</span><span class="p">,</span> <span class="kt">bool</span> <span class="n">isClosed</span><span class="p">,</span> <span class="kt">int</span> <span class="n">addSize</span><span class="p">,</span> <span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;&amp;</span> <span class="n">dps</span><span class="p">)</span></div><div class='line' id='LC197'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC198'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">double</span> <span class="n">addDelta</span> <span class="o">=</span> <span class="mf">1.0</span> <span class="o">/</span> <span class="p">(</span><span class="n">addSize</span> <span class="o">+</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC199'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">fpsSize</span> <span class="o">=</span> <span class="n">fps</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span><span class="p">;</span></div><div class='line' id='LC200'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dps</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC201'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">fpsSize</span><span class="p">;</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC202'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC203'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">hid</span> <span class="o">=</span> <span class="n">fps</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC204'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">wid</span> <span class="o">=</span> <span class="n">fps</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC205'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="o">!</span><span class="n">isClosed</span> <span class="o">&amp;&amp;</span> <span class="n">fid</span> <span class="o">==</span> <span class="n">fpsSize</span> <span class="o">-</span> <span class="mi">1</span><span class="p">)</span></div><div class='line' id='LC206'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC207'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dps</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="n">hid</span><span class="p">);</span></div><div class='line' id='LC208'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dps</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="n">wid</span><span class="p">);</span></div><div class='line' id='LC209'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">break</span><span class="p">;</span></div><div class='line' id='LC210'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC211'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">hidNext</span> <span class="o">=</span> <span class="n">fps</span><span class="p">.</span><span class="n">at</span><span class="p">((</span><span class="n">fid</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">%</span> <span class="n">fpsSize</span> <span class="o">*</span> <span class="mi">2</span><span class="p">);</span></div><div class='line' id='LC212'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">widNext</span> <span class="o">=</span> <span class="n">fps</span><span class="p">.</span><span class="n">at</span><span class="p">((</span><span class="n">fid</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">%</span> <span class="n">fpsSize</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC213'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">addId</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">addId</span> <span class="o">&lt;=</span> <span class="n">addSize</span><span class="p">;</span> <span class="n">addId</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC214'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC215'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">addHid</span> <span class="o">=</span> <span class="kt">int</span><span class="p">(</span><span class="n">hid</span> <span class="o">*</span> <span class="p">(</span><span class="mf">1.0</span> <span class="o">-</span> <span class="n">addId</span> <span class="o">*</span> <span class="n">addDelta</span><span class="p">)</span> <span class="o">+</span> <span class="n">hidNext</span> <span class="o">*</span> <span class="n">addId</span> <span class="o">*</span> <span class="n">addDelta</span><span class="p">);</span></div><div class='line' id='LC216'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">addWid</span> <span class="o">=</span> <span class="kt">int</span><span class="p">(</span><span class="n">wid</span> <span class="o">*</span> <span class="p">(</span><span class="mf">1.0</span> <span class="o">-</span> <span class="n">addId</span> <span class="o">*</span> <span class="n">addDelta</span><span class="p">)</span> <span class="o">+</span> <span class="n">widNext</span> <span class="o">*</span> <span class="n">addId</span> <span class="o">*</span> <span class="n">addDelta</span><span class="p">);</span></div><div class='line' id='LC217'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dps</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="n">addHid</span><span class="p">);</span></div><div class='line' id='LC218'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">dps</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="n">addWid</span><span class="p">);</span></div><div class='line' id='LC219'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC220'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC221'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC222'><br/></div><div class='line' id='LC223'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">void</span> <span class="n">FaceFeaturePoint</span><span class="o">::</span><span class="n">Save</span><span class="p">(</span><span class="k">const</span> <span class="n">std</span><span class="o">::</span><span class="n">string</span><span class="o">&amp;</span> <span class="n">fileName</span><span class="p">)</span></div><div class='line' id='LC224'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC225'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">std</span><span class="o">::</span><span class="n">ofstream</span> <span class="n">fout</span><span class="p">(</span><span class="n">fileName</span><span class="p">);</span></div><div class='line' id='LC226'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fout</span> <span class="o">&lt;&lt;</span> <span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span> <span class="o">&lt;&lt;</span> <span class="s">&quot; &quot;</span><span class="p">;</span></div><div class='line' id='LC227'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">size</span><span class="p">();</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC228'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC229'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fout</span> <span class="o">&lt;&lt;</span> <span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span><span class="p">)</span> <span class="o">&lt;&lt;</span> <span class="s">&quot; &quot;</span><span class="p">;</span></div><div class='line' id='LC230'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC231'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fout</span> <span class="o">&lt;&lt;</span> <span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span> <span class="o">&lt;&lt;</span> <span class="s">&quot; &quot;</span><span class="p">;</span></div><div class='line' id='LC232'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">size</span><span class="p">();</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC233'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC234'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fout</span> <span class="o">&lt;&lt;</span> <span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span><span class="p">)</span> <span class="o">&lt;&lt;</span> <span class="s">&quot; &quot;</span><span class="p">;</span></div><div class='line' id='LC235'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC236'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fout</span> <span class="o">&lt;&lt;</span> <span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span> <span class="o">&lt;&lt;</span> <span class="s">&quot; &quot;</span><span class="p">;</span></div><div class='line' id='LC237'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">size</span><span class="p">();</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC238'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC239'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fout</span> <span class="o">&lt;&lt;</span> <span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span><span class="p">)</span> <span class="o">&lt;&lt;</span> <span class="s">&quot; &quot;</span><span class="p">;</span></div><div class='line' id='LC240'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC241'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fout</span> <span class="o">&lt;&lt;</span> <span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span> <span class="o">&lt;&lt;</span> <span class="s">&quot; &quot;</span><span class="p">;</span></div><div class='line' id='LC242'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">size</span><span class="p">();</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC243'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC244'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fout</span> <span class="o">&lt;&lt;</span> <span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span><span class="p">)</span> <span class="o">&lt;&lt;</span> <span class="s">&quot; &quot;</span><span class="p">;</span></div><div class='line' id='LC245'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC246'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fout</span> <span class="o">&lt;&lt;</span> <span class="n">mNoseFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span> <span class="o">&lt;&lt;</span> <span class="s">&quot; &quot;</span><span class="p">;</span></div><div class='line' id='LC247'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">mNoseFPs</span><span class="p">.</span><span class="n">size</span><span class="p">();</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC248'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC249'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fout</span> <span class="o">&lt;&lt;</span> <span class="n">mNoseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span><span class="p">)</span> <span class="o">&lt;&lt;</span> <span class="s">&quot; &quot;</span><span class="p">;</span></div><div class='line' id='LC250'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC251'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fout</span> <span class="o">&lt;&lt;</span> <span class="n">mMouseFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span> <span class="o">&lt;&lt;</span> <span class="s">&quot; &quot;</span><span class="p">;</span></div><div class='line' id='LC252'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">mMouseFPs</span><span class="p">.</span><span class="n">size</span><span class="p">();</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC253'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC254'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fout</span> <span class="o">&lt;&lt;</span> <span class="n">mMouseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span><span class="p">)</span> <span class="o">&lt;&lt;</span> <span class="s">&quot; &quot;</span><span class="p">;</span></div><div class='line' id='LC255'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC256'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fout</span> <span class="o">&lt;&lt;</span> <span class="n">mBorderFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span> <span class="o">&lt;&lt;</span> <span class="s">&quot; &quot;</span><span class="p">;</span></div><div class='line' id='LC257'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">mBorderFPs</span><span class="p">.</span><span class="n">size</span><span class="p">();</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC258'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC259'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fout</span> <span class="o">&lt;&lt;</span> <span class="n">mBorderFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">fid</span><span class="p">)</span> <span class="o">&lt;&lt;</span> <span class="s">&quot; &quot;</span><span class="p">;</span></div><div class='line' id='LC260'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC261'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">fout</span><span class="p">.</span><span class="n">close</span><span class="p">();</span></div><div class='line' id='LC262'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC263'><br/></div><div class='line' id='LC264'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">bool</span> <span class="n">FaceFeaturePoint</span><span class="o">::</span><span class="n">Select</span><span class="p">(</span><span class="kt">int</span> <span class="n">hid</span><span class="p">,</span> <span class="kt">int</span> <span class="n">wid</span><span class="p">)</span></div><div class='line' id='LC265'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC266'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">tol</span> <span class="o">=</span> <span class="mi">5</span><span class="p">;</span></div><div class='line' id='LC267'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span><span class="p">;</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC268'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC269'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">abs</span><span class="p">(</span><span class="n">hid</span> <span class="o">-</span> <span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="mi">2</span> <span class="o">*</span> <span class="n">fid</span><span class="p">))</span> <span class="o">&lt;</span> <span class="n">tol</span> <span class="o">&amp;&amp;</span></div><div class='line' id='LC270'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">abs</span><span class="p">(</span><span class="n">wid</span> <span class="o">-</span> <span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="mi">2</span> <span class="o">*</span> <span class="n">fid</span> <span class="o">+</span> <span class="mi">1</span><span class="p">))</span> <span class="o">&lt;</span> <span class="n">tol</span><span class="p">)</span></div><div class='line' id='LC271'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC272'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectType</span> <span class="o">=</span> <span class="n">FT_Left_Brow</span><span class="p">;</span></div><div class='line' id='LC273'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectIndex</span> <span class="o">=</span> <span class="n">fid</span><span class="p">;</span></div><div class='line' id='LC274'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="nb">true</span><span class="p">;</span></div><div class='line' id='LC275'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC276'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC277'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span><span class="p">;</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC278'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC279'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">abs</span><span class="p">(</span><span class="n">hid</span> <span class="o">-</span> <span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="mi">2</span> <span class="o">*</span> <span class="n">fid</span><span class="p">))</span> <span class="o">&lt;</span> <span class="n">tol</span> <span class="o">&amp;&amp;</span></div><div class='line' id='LC280'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">abs</span><span class="p">(</span><span class="n">wid</span> <span class="o">-</span> <span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="mi">2</span> <span class="o">*</span> <span class="n">fid</span> <span class="o">+</span> <span class="mi">1</span><span class="p">))</span> <span class="o">&lt;</span> <span class="n">tol</span><span class="p">)</span></div><div class='line' id='LC281'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC282'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectType</span> <span class="o">=</span> <span class="n">FT_Right_Brow</span><span class="p">;</span></div><div class='line' id='LC283'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectIndex</span> <span class="o">=</span> <span class="n">fid</span><span class="p">;</span></div><div class='line' id='LC284'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="nb">true</span><span class="p">;</span></div><div class='line' id='LC285'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC286'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC287'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span><span class="p">;</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC288'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC289'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">abs</span><span class="p">(</span><span class="n">hid</span> <span class="o">-</span> <span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="mi">2</span> <span class="o">*</span> <span class="n">fid</span><span class="p">))</span> <span class="o">&lt;</span> <span class="n">tol</span> <span class="o">&amp;&amp;</span></div><div class='line' id='LC290'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">abs</span><span class="p">(</span><span class="n">wid</span> <span class="o">-</span> <span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="mi">2</span> <span class="o">*</span> <span class="n">fid</span> <span class="o">+</span> <span class="mi">1</span><span class="p">))</span> <span class="o">&lt;</span> <span class="n">tol</span><span class="p">)</span></div><div class='line' id='LC291'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC292'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectType</span> <span class="o">=</span> <span class="n">FT_Left_Eye</span><span class="p">;</span></div><div class='line' id='LC293'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectIndex</span> <span class="o">=</span> <span class="n">fid</span><span class="p">;</span></div><div class='line' id='LC294'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="nb">true</span><span class="p">;</span></div><div class='line' id='LC295'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC296'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC297'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span><span class="p">;</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC298'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC299'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">abs</span><span class="p">(</span><span class="n">hid</span> <span class="o">-</span> <span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="mi">2</span> <span class="o">*</span> <span class="n">fid</span><span class="p">))</span> <span class="o">&lt;</span> <span class="n">tol</span> <span class="o">&amp;&amp;</span></div><div class='line' id='LC300'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">abs</span><span class="p">(</span><span class="n">wid</span> <span class="o">-</span> <span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="mi">2</span> <span class="o">*</span> <span class="n">fid</span> <span class="o">+</span> <span class="mi">1</span><span class="p">))</span> <span class="o">&lt;</span> <span class="n">tol</span><span class="p">)</span></div><div class='line' id='LC301'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC302'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectType</span> <span class="o">=</span> <span class="n">FT_Right_Eye</span><span class="p">;</span></div><div class='line' id='LC303'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectIndex</span> <span class="o">=</span> <span class="n">fid</span><span class="p">;</span></div><div class='line' id='LC304'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="nb">true</span><span class="p">;</span></div><div class='line' id='LC305'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC306'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC307'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">mNoseFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span><span class="p">;</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC308'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC309'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">abs</span><span class="p">(</span><span class="n">hid</span> <span class="o">-</span> <span class="n">mNoseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="mi">2</span> <span class="o">*</span> <span class="n">fid</span><span class="p">))</span> <span class="o">&lt;</span> <span class="n">tol</span> <span class="o">&amp;&amp;</span></div><div class='line' id='LC310'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">abs</span><span class="p">(</span><span class="n">wid</span> <span class="o">-</span> <span class="n">mNoseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="mi">2</span> <span class="o">*</span> <span class="n">fid</span> <span class="o">+</span> <span class="mi">1</span><span class="p">))</span> <span class="o">&lt;</span> <span class="n">tol</span><span class="p">)</span></div><div class='line' id='LC311'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC312'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectType</span> <span class="o">=</span> <span class="n">FT_Nose</span><span class="p">;</span></div><div class='line' id='LC313'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectIndex</span> <span class="o">=</span> <span class="n">fid</span><span class="p">;</span></div><div class='line' id='LC314'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="nb">true</span><span class="p">;</span></div><div class='line' id='LC315'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC316'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC317'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">mMouseFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span><span class="p">;</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC318'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC319'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">abs</span><span class="p">(</span><span class="n">hid</span> <span class="o">-</span> <span class="n">mMouseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="mi">2</span> <span class="o">*</span> <span class="n">fid</span><span class="p">))</span> <span class="o">&lt;</span> <span class="n">tol</span> <span class="o">&amp;&amp;</span></div><div class='line' id='LC320'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">abs</span><span class="p">(</span><span class="n">wid</span> <span class="o">-</span> <span class="n">mMouseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="mi">2</span> <span class="o">*</span> <span class="n">fid</span> <span class="o">+</span> <span class="mi">1</span><span class="p">))</span> <span class="o">&lt;</span> <span class="n">tol</span><span class="p">)</span></div><div class='line' id='LC321'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC322'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectType</span> <span class="o">=</span> <span class="n">FT_Mouse</span><span class="p">;</span></div><div class='line' id='LC323'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectIndex</span> <span class="o">=</span> <span class="n">fid</span><span class="p">;</span></div><div class='line' id='LC324'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="nb">true</span><span class="p">;</span></div><div class='line' id='LC325'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC326'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC327'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="kt">int</span> <span class="n">fid</span> <span class="o">=</span> <span class="mi">0</span><span class="p">;</span> <span class="n">fid</span> <span class="o">&lt;</span> <span class="n">mBorderFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span><span class="p">;</span> <span class="n">fid</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC328'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC329'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">abs</span><span class="p">(</span><span class="n">hid</span> <span class="o">-</span> <span class="n">mBorderFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="mi">2</span> <span class="o">*</span> <span class="n">fid</span><span class="p">))</span> <span class="o">&lt;</span> <span class="n">tol</span> <span class="o">&amp;&amp;</span></div><div class='line' id='LC330'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">abs</span><span class="p">(</span><span class="n">wid</span> <span class="o">-</span> <span class="n">mBorderFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="mi">2</span> <span class="o">*</span> <span class="n">fid</span> <span class="o">+</span> <span class="mi">1</span><span class="p">))</span> <span class="o">&lt;</span> <span class="n">tol</span><span class="p">)</span></div><div class='line' id='LC331'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC332'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectType</span> <span class="o">=</span> <span class="n">FT_Border</span><span class="p">;</span></div><div class='line' id='LC333'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectIndex</span> <span class="o">=</span> <span class="n">fid</span><span class="p">;</span></div><div class='line' id='LC334'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="nb">true</span><span class="p">;</span></div><div class='line' id='LC335'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC336'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC337'><br/></div><div class='line' id='LC338'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectType</span> <span class="o">=</span> <span class="n">FT_None</span><span class="p">;</span></div><div class='line' id='LC339'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mSelectIndex</span> <span class="o">=</span> <span class="o">-</span><span class="mi">1</span><span class="p">;</span></div><div class='line' id='LC340'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="nb">false</span><span class="p">;</span></div><div class='line' id='LC341'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC342'><br/></div><div class='line' id='LC343'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">void</span> <span class="n">FaceFeaturePoint</span><span class="o">::</span><span class="n">MoveTo</span><span class="p">(</span><span class="kt">int</span> <span class="n">hid</span><span class="p">,</span> <span class="kt">int</span> <span class="n">wid</span><span class="p">)</span></div><div class='line' id='LC344'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC345'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">mSelectType</span> <span class="o">==</span> <span class="n">FT_Left_Brow</span><span class="p">)</span></div><div class='line' id='LC346'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC347'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">hid</span><span class="p">;</span></div><div class='line' id='LC348'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC349'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC350'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">else</span> <span class="k">if</span> <span class="p">(</span><span class="n">mSelectType</span> <span class="o">==</span> <span class="n">FT_Right_Brow</span><span class="p">)</span></div><div class='line' id='LC351'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC352'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">hid</span><span class="p">;</span></div><div class='line' id='LC353'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC354'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC355'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">else</span> <span class="k">if</span> <span class="p">(</span><span class="n">mSelectType</span> <span class="o">==</span> <span class="n">FT_Left_Eye</span><span class="p">)</span></div><div class='line' id='LC356'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC357'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">hid</span><span class="p">;</span></div><div class='line' id='LC358'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC359'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC360'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">else</span> <span class="k">if</span> <span class="p">(</span><span class="n">mSelectType</span> <span class="o">==</span> <span class="n">FT_Right_Eye</span><span class="p">)</span></div><div class='line' id='LC361'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC362'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">hid</span><span class="p">;</span></div><div class='line' id='LC363'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC364'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC365'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">else</span> <span class="k">if</span> <span class="p">(</span><span class="n">mSelectType</span> <span class="o">==</span> <span class="n">FT_Nose</span><span class="p">)</span></div><div class='line' id='LC366'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC367'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mNoseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">hid</span><span class="p">;</span></div><div class='line' id='LC368'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mNoseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC369'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC370'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">else</span> <span class="k">if</span> <span class="p">(</span><span class="n">mSelectType</span> <span class="o">==</span> <span class="n">FT_Mouse</span><span class="p">)</span></div><div class='line' id='LC371'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC372'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mMouseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">hid</span><span class="p">;</span></div><div class='line' id='LC373'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mMouseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC374'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC375'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">else</span> <span class="k">if</span> <span class="p">(</span><span class="n">mSelectType</span> <span class="o">==</span> <span class="n">FT_Border</span><span class="p">)</span></div><div class='line' id='LC376'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC377'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mBorderFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">=</span> <span class="n">hid</span><span class="p">;</span></div><div class='line' id='LC378'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mBorderFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">=</span> <span class="n">wid</span><span class="p">;</span></div><div class='line' id='LC379'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC380'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">UpdateDPs</span><span class="p">();</span></div><div class='line' id='LC381'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC382'><br/></div><div class='line' id='LC383'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">void</span> <span class="n">FaceFeaturePoint</span><span class="o">::</span><span class="n">MoveDelta</span><span class="p">(</span><span class="kt">int</span> <span class="n">deltaH</span><span class="p">,</span> <span class="kt">int</span> <span class="n">deltaW</span><span class="p">)</span></div><div class='line' id='LC384'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC385'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">mSelectType</span> <span class="o">==</span> <span class="n">FT_Left_Brow</span><span class="p">)</span></div><div class='line' id='LC386'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC387'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">+=</span> <span class="n">deltaH</span><span class="p">;</span></div><div class='line' id='LC388'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">+=</span> <span class="n">deltaW</span><span class="p">;</span></div><div class='line' id='LC389'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC390'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">else</span> <span class="k">if</span> <span class="p">(</span><span class="n">mSelectType</span> <span class="o">==</span> <span class="n">FT_Right_Brow</span><span class="p">)</span></div><div class='line' id='LC391'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC392'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">+=</span> <span class="n">deltaH</span><span class="p">;</span></div><div class='line' id='LC393'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">+=</span> <span class="n">deltaW</span><span class="p">;</span></div><div class='line' id='LC394'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC395'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">else</span> <span class="k">if</span> <span class="p">(</span><span class="n">mSelectType</span> <span class="o">==</span> <span class="n">FT_Left_Eye</span><span class="p">)</span></div><div class='line' id='LC396'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC397'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">+=</span> <span class="n">deltaH</span><span class="p">;</span></div><div class='line' id='LC398'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">+=</span> <span class="n">deltaW</span><span class="p">;</span></div><div class='line' id='LC399'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC400'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">else</span> <span class="k">if</span> <span class="p">(</span><span class="n">mSelectType</span> <span class="o">==</span> <span class="n">FT_Right_Eye</span><span class="p">)</span></div><div class='line' id='LC401'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC402'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">+=</span> <span class="n">deltaH</span><span class="p">;</span></div><div class='line' id='LC403'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">+=</span> <span class="n">deltaW</span><span class="p">;</span></div><div class='line' id='LC404'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC405'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">else</span> <span class="k">if</span> <span class="p">(</span><span class="n">mSelectType</span> <span class="o">==</span> <span class="n">FT_Nose</span><span class="p">)</span></div><div class='line' id='LC406'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC407'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mNoseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">+=</span> <span class="n">deltaH</span><span class="p">;</span></div><div class='line' id='LC408'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mNoseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">+=</span> <span class="n">deltaW</span><span class="p">;</span></div><div class='line' id='LC409'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC410'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">else</span> <span class="k">if</span> <span class="p">(</span><span class="n">mSelectType</span> <span class="o">==</span> <span class="n">FT_Mouse</span><span class="p">)</span></div><div class='line' id='LC411'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC412'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mMouseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">+=</span> <span class="n">deltaH</span><span class="p">;</span></div><div class='line' id='LC413'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mMouseFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">+=</span> <span class="n">deltaW</span><span class="p">;</span></div><div class='line' id='LC414'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC415'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">else</span> <span class="k">if</span> <span class="p">(</span><span class="n">mSelectType</span> <span class="o">==</span> <span class="n">FT_Border</span><span class="p">)</span></div><div class='line' id='LC416'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC417'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mBorderFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span><span class="p">)</span> <span class="o">+=</span> <span class="n">deltaH</span><span class="p">;</span></div><div class='line' id='LC418'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mBorderFPs</span><span class="p">.</span><span class="n">at</span><span class="p">(</span><span class="n">mSelectIndex</span> <span class="o">*</span> <span class="mi">2</span> <span class="o">+</span> <span class="mi">1</span><span class="p">)</span> <span class="o">+=</span> <span class="n">deltaW</span><span class="p">;</span></div><div class='line' id='LC419'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC420'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">UpdateDPs</span><span class="p">();</span></div><div class='line' id='LC421'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC422'><br/></div><div class='line' id='LC423'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">void</span> <span class="n">FaceFeaturePoint</span><span class="o">::</span><span class="n">GetDPs</span><span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;&amp;</span> <span class="n">posList</span><span class="p">)</span></div><div class='line' id='LC424'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC425'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC426'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;::</span><span class="n">iterator</span> <span class="n">itr</span> <span class="o">=</span> <span class="n">mLeftBrowDPs</span><span class="p">.</span><span class="n">begin</span><span class="p">();</span> <span class="n">itr</span> <span class="o">!=</span> <span class="n">mLeftBrowDPs</span><span class="p">.</span><span class="n">end</span><span class="p">();</span> <span class="n">itr</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC427'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC428'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="o">*</span><span class="n">itr</span><span class="p">);</span></div><div class='line' id='LC429'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC430'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;::</span><span class="n">iterator</span> <span class="n">itr</span> <span class="o">=</span> <span class="n">mRightBrowDPs</span><span class="p">.</span><span class="n">begin</span><span class="p">();</span> <span class="n">itr</span> <span class="o">!=</span> <span class="n">mRightBrowDPs</span><span class="p">.</span><span class="n">end</span><span class="p">();</span> <span class="n">itr</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC431'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC432'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="o">*</span><span class="n">itr</span><span class="p">);</span></div><div class='line' id='LC433'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC434'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;::</span><span class="n">iterator</span> <span class="n">itr</span> <span class="o">=</span> <span class="n">mLeftEyeDPs</span><span class="p">.</span><span class="n">begin</span><span class="p">();</span> <span class="n">itr</span> <span class="o">!=</span> <span class="n">mLeftEyeDPs</span><span class="p">.</span><span class="n">end</span><span class="p">();</span> <span class="n">itr</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC435'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC436'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="o">*</span><span class="n">itr</span><span class="p">);</span></div><div class='line' id='LC437'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC438'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;::</span><span class="n">iterator</span> <span class="n">itr</span> <span class="o">=</span> <span class="n">mRightEyeDPs</span><span class="p">.</span><span class="n">begin</span><span class="p">();</span> <span class="n">itr</span> <span class="o">!=</span> <span class="n">mRightEyeDPs</span><span class="p">.</span><span class="n">end</span><span class="p">();</span> <span class="n">itr</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC439'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC440'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="o">*</span><span class="n">itr</span><span class="p">);</span></div><div class='line' id='LC441'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC442'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;::</span><span class="n">iterator</span> <span class="n">itr</span> <span class="o">=</span> <span class="n">mNoseDPs</span><span class="p">.</span><span class="n">begin</span><span class="p">();</span> <span class="n">itr</span> <span class="o">!=</span> <span class="n">mNoseDPs</span><span class="p">.</span><span class="n">end</span><span class="p">();</span> <span class="n">itr</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC443'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC444'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="o">*</span><span class="n">itr</span><span class="p">);</span></div><div class='line' id='LC445'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC446'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;::</span><span class="n">iterator</span> <span class="n">itr</span> <span class="o">=</span> <span class="n">mMouseDPs</span><span class="p">.</span><span class="n">begin</span><span class="p">();</span> <span class="n">itr</span> <span class="o">!=</span> <span class="n">mMouseDPs</span><span class="p">.</span><span class="n">end</span><span class="p">();</span> <span class="n">itr</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC447'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC448'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="o">*</span><span class="n">itr</span><span class="p">);</span></div><div class='line' id='LC449'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC450'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;::</span><span class="n">iterator</span> <span class="n">itr</span> <span class="o">=</span> <span class="n">mBorderDPs</span><span class="p">.</span><span class="n">begin</span><span class="p">();</span> <span class="n">itr</span> <span class="o">!=</span> <span class="n">mBorderDPs</span><span class="p">.</span><span class="n">end</span><span class="p">();</span> <span class="n">itr</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC451'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC452'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="o">*</span><span class="n">itr</span><span class="p">);</span></div><div class='line' id='LC453'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC454'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC455'><br/></div><div class='line' id='LC456'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">void</span> <span class="n">FaceFeaturePoint</span><span class="o">::</span><span class="n">GetFPs</span><span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;&amp;</span> <span class="n">posList</span><span class="p">)</span></div><div class='line' id='LC457'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC458'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">clear</span><span class="p">();</span></div><div class='line' id='LC459'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;::</span><span class="n">iterator</span> <span class="n">itr</span> <span class="o">=</span> <span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">begin</span><span class="p">();</span> <span class="n">itr</span> <span class="o">!=</span> <span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">end</span><span class="p">();</span> <span class="n">itr</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC460'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC461'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="o">*</span><span class="n">itr</span><span class="p">);</span></div><div class='line' id='LC462'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC463'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;::</span><span class="n">iterator</span> <span class="n">itr</span> <span class="o">=</span> <span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">begin</span><span class="p">();</span> <span class="n">itr</span> <span class="o">!=</span> <span class="n">mRightBrowFPs</span><span class="p">.</span><span class="n">end</span><span class="p">();</span> <span class="n">itr</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC464'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC465'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="o">*</span><span class="n">itr</span><span class="p">);</span></div><div class='line' id='LC466'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC467'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;::</span><span class="n">iterator</span> <span class="n">itr</span> <span class="o">=</span> <span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">begin</span><span class="p">();</span> <span class="n">itr</span> <span class="o">!=</span> <span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">end</span><span class="p">();</span> <span class="n">itr</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC468'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC469'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="o">*</span><span class="n">itr</span><span class="p">);</span></div><div class='line' id='LC470'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC471'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;::</span><span class="n">iterator</span> <span class="n">itr</span> <span class="o">=</span> <span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">begin</span><span class="p">();</span> <span class="n">itr</span> <span class="o">!=</span> <span class="n">mRightEyeFPs</span><span class="p">.</span><span class="n">end</span><span class="p">();</span> <span class="n">itr</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC472'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC473'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="o">*</span><span class="n">itr</span><span class="p">);</span></div><div class='line' id='LC474'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC475'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;::</span><span class="n">iterator</span> <span class="n">itr</span> <span class="o">=</span> <span class="n">mNoseFPs</span><span class="p">.</span><span class="n">begin</span><span class="p">();</span> <span class="n">itr</span> <span class="o">!=</span> <span class="n">mNoseFPs</span><span class="p">.</span><span class="n">end</span><span class="p">();</span> <span class="n">itr</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC476'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC477'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="o">*</span><span class="n">itr</span><span class="p">);</span></div><div class='line' id='LC478'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC479'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;::</span><span class="n">iterator</span> <span class="n">itr</span> <span class="o">=</span> <span class="n">mMouseFPs</span><span class="p">.</span><span class="n">begin</span><span class="p">();</span> <span class="n">itr</span> <span class="o">!=</span> <span class="n">mMouseFPs</span><span class="p">.</span><span class="n">end</span><span class="p">();</span> <span class="n">itr</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC480'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC481'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="o">*</span><span class="n">itr</span><span class="p">);</span></div><div class='line' id='LC482'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC483'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">for</span> <span class="p">(</span><span class="n">std</span><span class="o">::</span><span class="n">vector</span><span class="o">&lt;</span><span class="kt">int</span><span class="o">&gt;::</span><span class="n">iterator</span> <span class="n">itr</span> <span class="o">=</span> <span class="n">mBorderFPs</span><span class="p">.</span><span class="n">begin</span><span class="p">();</span> <span class="n">itr</span> <span class="o">!=</span> <span class="n">mBorderFPs</span><span class="p">.</span><span class="n">end</span><span class="p">();</span> <span class="n">itr</span><span class="o">++</span><span class="p">)</span></div><div class='line' id='LC484'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC485'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">posList</span><span class="p">.</span><span class="n">push_back</span><span class="p">(</span><span class="o">*</span><span class="n">itr</span><span class="p">);</span></div><div class='line' id='LC486'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC487'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC488'><br/></div><div class='line' id='LC489'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">void</span> <span class="n">FaceFeaturePoint</span><span class="o">::</span><span class="n">GetParameter</span><span class="p">(</span><span class="kt">int</span><span class="o">&amp;</span> <span class="n">browNum</span><span class="p">,</span> <span class="kt">int</span><span class="o">&amp;</span> <span class="n">eyeNum</span><span class="p">,</span> <span class="kt">int</span><span class="o">&amp;</span> <span class="n">noseNum</span><span class="p">,</span> <span class="kt">int</span><span class="o">&amp;</span> <span class="n">mouseNum</span><span class="p">,</span> <span class="kt">int</span><span class="o">&amp;</span> <span class="n">borderNum</span><span class="p">)</span></div><div class='line' id='LC490'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC491'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">browNum</span> <span class="o">=</span> <span class="n">mLeftBrowFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span><span class="p">;</span></div><div class='line' id='LC492'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">eyeNum</span> <span class="o">=</span> <span class="n">mLeftEyeFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span><span class="p">;</span></div><div class='line' id='LC493'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">noseNum</span> <span class="o">=</span> <span class="n">mNoseFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span><span class="p">;</span></div><div class='line' id='LC494'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mouseNum</span> <span class="o">=</span> <span class="n">mMouseFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span><span class="p">;</span></div><div class='line' id='LC495'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">borderNum</span> <span class="o">=</span> <span class="n">mBorderFPs</span><span class="p">.</span><span class="n">size</span><span class="p">()</span> <span class="o">/</span> <span class="mi">2</span><span class="p">;</span></div><div class='line' id='LC496'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC497'><br/></div><div class='line' id='LC498'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">Face2D</span><span class="o">::</span><span class="n">Face2D</span><span class="p">()</span></div><div class='line' id='LC499'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC500'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC501'><br/></div><div class='line' id='LC502'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">Face2D</span><span class="o">::~</span><span class="n">Face2D</span><span class="p">()</span></div><div class='line' id='LC503'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC504'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mImage</span><span class="p">.</span><span class="n">release</span><span class="p">();</span></div><div class='line' id='LC505'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRefImage</span><span class="p">.</span><span class="n">release</span><span class="p">();</span></div><div class='line' id='LC506'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC507'><br/></div><div class='line' id='LC508'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">bool</span> <span class="n">Face2D</span><span class="o">::</span><span class="n">LoadImage</span><span class="p">(</span><span class="k">const</span> <span class="n">std</span><span class="o">::</span><span class="n">string</span><span class="o">&amp;</span> <span class="n">fileName</span><span class="p">)</span></div><div class='line' id='LC509'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC510'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mImage</span><span class="p">.</span><span class="n">release</span><span class="p">();</span></div><div class='line' id='LC511'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mImage</span> <span class="o">=</span> <span class="n">cv</span><span class="o">::</span><span class="n">imread</span><span class="p">(</span><span class="n">fileName</span><span class="p">);</span></div><div class='line' id='LC512'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">mImage</span><span class="p">.</span><span class="n">data</span> <span class="o">!=</span> <span class="nb">NULL</span><span class="p">)</span></div><div class='line' id='LC513'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC514'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="nb">true</span><span class="p">;</span></div><div class='line' id='LC515'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC516'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">else</span></div><div class='line' id='LC517'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC518'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="nb">false</span><span class="p">;</span></div><div class='line' id='LC519'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC520'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC521'><br/></div><div class='line' id='LC522'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">cv</span><span class="o">::</span><span class="n">Mat</span> <span class="n">Face2D</span><span class="o">::</span><span class="n">GetImage</span><span class="p">(</span><span class="kt">void</span><span class="p">)</span></div><div class='line' id='LC523'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC524'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="n">mImage</span><span class="p">;</span></div><div class='line' id='LC525'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC526'><br/></div><div class='line' id='LC527'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">bool</span> <span class="n">Face2D</span><span class="o">::</span><span class="n">LoadFps</span><span class="p">(</span><span class="k">const</span> <span class="n">std</span><span class="o">::</span><span class="n">string</span><span class="o">&amp;</span> <span class="n">fileName</span><span class="p">)</span></div><div class='line' id='LC528'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC529'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="n">mFps</span><span class="p">.</span><span class="n">Load</span><span class="p">(</span><span class="n">fileName</span><span class="p">);</span></div><div class='line' id='LC530'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC531'><br/></div><div class='line' id='LC532'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">FaceFeaturePoint</span><span class="o">*</span> <span class="n">Face2D</span><span class="o">::</span><span class="n">GetFps</span><span class="p">(</span><span class="kt">void</span><span class="p">)</span></div><div class='line' id='LC533'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC534'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="o">&amp;</span><span class="n">mFps</span><span class="p">;</span></div><div class='line' id='LC535'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC536'><br/></div><div class='line' id='LC537'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">bool</span> <span class="n">Face2D</span><span class="o">::</span><span class="n">LoadRefImage</span><span class="p">(</span><span class="k">const</span> <span class="n">std</span><span class="o">::</span><span class="n">string</span><span class="o">&amp;</span> <span class="n">fileName</span><span class="p">)</span></div><div class='line' id='LC538'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC539'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRefImage</span><span class="p">.</span><span class="n">release</span><span class="p">();</span></div><div class='line' id='LC540'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRefImage</span> <span class="o">=</span> <span class="n">cv</span><span class="o">::</span><span class="n">imread</span><span class="p">(</span><span class="n">fileName</span><span class="p">);</span></div><div class='line' id='LC541'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">mRefImage</span><span class="p">.</span><span class="n">data</span> <span class="o">!=</span> <span class="nb">NULL</span><span class="p">)</span></div><div class='line' id='LC542'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC543'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="nb">true</span><span class="p">;</span></div><div class='line' id='LC544'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC545'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">else</span></div><div class='line' id='LC546'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC547'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="nb">false</span><span class="p">;</span></div><div class='line' id='LC548'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC549'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC550'><br/></div><div class='line' id='LC551'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">cv</span><span class="o">::</span><span class="n">Mat</span> <span class="n">Face2D</span><span class="o">::</span><span class="n">GetRefImage</span><span class="p">(</span><span class="kt">void</span><span class="p">)</span></div><div class='line' id='LC552'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC553'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="n">mRefImage</span><span class="p">;</span></div><div class='line' id='LC554'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC555'><br/></div><div class='line' id='LC556'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">bool</span> <span class="n">Face2D</span><span class="o">::</span><span class="n">LoadrefFps</span><span class="p">(</span><span class="k">const</span> <span class="n">std</span><span class="o">::</span><span class="n">string</span><span class="o">&amp;</span> <span class="n">fileName</span><span class="p">)</span></div><div class='line' id='LC557'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC558'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="n">mRefFps</span><span class="p">.</span><span class="n">Load</span><span class="p">(</span><span class="n">fileName</span><span class="p">);</span></div><div class='line' id='LC559'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC560'><br/></div><div class='line' id='LC561'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">FaceFeaturePoint</span><span class="o">*</span> <span class="n">Face2D</span><span class="o">::</span><span class="n">GetRefFps</span><span class="p">(</span><span class="kt">void</span><span class="p">)</span></div><div class='line' id='LC562'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC563'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">return</span> <span class="o">&amp;</span><span class="n">mRefFps</span><span class="p">;</span></div><div class='line' id='LC564'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC565'><br/></div><div class='line' id='LC566'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">void</span> <span class="n">Face2D</span><span class="o">::</span><span class="n">SetMaxImageSize</span><span class="p">(</span><span class="kt">int</span> <span class="n">width</span><span class="p">,</span> <span class="kt">int</span> <span class="n">height</span><span class="p">)</span></div><div class='line' id='LC567'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC568'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">imgW</span> <span class="o">=</span> <span class="n">mImage</span><span class="p">.</span><span class="n">cols</span><span class="p">;</span></div><div class='line' id='LC569'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">imgH</span> <span class="o">=</span> <span class="n">mImage</span><span class="p">.</span><span class="n">rows</span><span class="p">;</span></div><div class='line' id='LC570'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">bool</span> <span class="n">resized</span> <span class="o">=</span> <span class="nb">false</span><span class="p">;</span></div><div class='line' id='LC571'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">imgW</span> <span class="o">&gt;</span> <span class="n">width</span><span class="p">)</span></div><div class='line' id='LC572'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC573'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">imgH</span> <span class="o">=</span> <span class="kt">int</span><span class="p">(</span><span class="n">imgH</span> <span class="o">*</span> <span class="kt">float</span><span class="p">(</span><span class="n">width</span><span class="p">)</span> <span class="o">/</span> <span class="n">imgW</span><span class="p">);</span></div><div class='line' id='LC574'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">imgW</span> <span class="o">=</span> <span class="n">width</span><span class="p">;</span></div><div class='line' id='LC575'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">resized</span> <span class="o">=</span> <span class="nb">true</span><span class="p">;</span></div><div class='line' id='LC576'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC577'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">imgH</span> <span class="o">&gt;</span> <span class="n">height</span><span class="p">)</span></div><div class='line' id='LC578'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC579'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">imgW</span> <span class="o">=</span> <span class="kt">int</span><span class="p">(</span><span class="n">imgW</span> <span class="o">*</span> <span class="kt">float</span><span class="p">(</span><span class="n">height</span><span class="p">)</span> <span class="o">/</span> <span class="n">imgH</span><span class="p">);</span></div><div class='line' id='LC580'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">imgH</span> <span class="o">=</span> <span class="n">height</span><span class="p">;</span></div><div class='line' id='LC581'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">resized</span> <span class="o">=</span> <span class="nb">true</span><span class="p">;</span></div><div class='line' id='LC582'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC583'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">resized</span><span class="p">)</span></div><div class='line' id='LC584'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC585'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">cv</span><span class="o">::</span><span class="n">Size</span> <span class="n">vcSize</span><span class="p">(</span><span class="n">imgW</span><span class="p">,</span> <span class="n">imgH</span><span class="p">);</span></div><div class='line' id='LC586'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">cv</span><span class="o">::</span><span class="n">Mat</span> <span class="n">resizedImg</span><span class="p">(</span><span class="n">vcSize</span><span class="p">,</span> <span class="n">CV_8UC3</span><span class="p">);</span></div><div class='line' id='LC587'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">cv</span><span class="o">::</span><span class="n">resize</span><span class="p">(</span><span class="n">mImage</span><span class="p">,</span> <span class="n">resizedImg</span><span class="p">,</span> <span class="n">vcSize</span><span class="p">);</span></div><div class='line' id='LC588'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mImage</span><span class="p">.</span><span class="n">release</span><span class="p">();</span></div><div class='line' id='LC589'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mImage</span> <span class="o">=</span> <span class="n">resizedImg</span><span class="p">.</span><span class="n">clone</span><span class="p">();</span></div><div class='line' id='LC590'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC591'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC592'><br/></div><div class='line' id='LC593'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">void</span> <span class="n">Face2D</span><span class="o">::</span><span class="n">SetMaxRefImageSize</span><span class="p">(</span><span class="kt">int</span> <span class="n">width</span><span class="p">,</span> <span class="kt">int</span> <span class="n">height</span><span class="p">)</span></div><div class='line' id='LC594'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC595'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">imgW</span> <span class="o">=</span> <span class="n">mRefImage</span><span class="p">.</span><span class="n">cols</span><span class="p">;</span></div><div class='line' id='LC596'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">int</span> <span class="n">imgH</span> <span class="o">=</span> <span class="n">mRefImage</span><span class="p">.</span><span class="n">rows</span><span class="p">;</span></div><div class='line' id='LC597'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="kt">bool</span> <span class="n">resized</span> <span class="o">=</span> <span class="nb">false</span><span class="p">;</span></div><div class='line' id='LC598'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">imgW</span> <span class="o">&gt;</span> <span class="n">width</span><span class="p">)</span></div><div class='line' id='LC599'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC600'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">imgH</span> <span class="o">=</span> <span class="kt">int</span><span class="p">(</span><span class="n">imgH</span> <span class="o">*</span> <span class="kt">float</span><span class="p">(</span><span class="n">width</span><span class="p">)</span> <span class="o">/</span> <span class="n">imgW</span><span class="p">);</span></div><div class='line' id='LC601'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">imgW</span> <span class="o">=</span> <span class="n">width</span><span class="p">;</span></div><div class='line' id='LC602'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">resized</span> <span class="o">=</span> <span class="nb">true</span><span class="p">;</span></div><div class='line' id='LC603'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC604'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">imgH</span> <span class="o">&gt;</span> <span class="n">height</span><span class="p">)</span></div><div class='line' id='LC605'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC606'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">imgW</span> <span class="o">=</span> <span class="kt">int</span><span class="p">(</span><span class="n">imgW</span> <span class="o">*</span> <span class="kt">float</span><span class="p">(</span><span class="n">height</span><span class="p">)</span> <span class="o">/</span> <span class="n">imgH</span><span class="p">);</span></div><div class='line' id='LC607'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">imgH</span> <span class="o">=</span> <span class="n">height</span><span class="p">;</span></div><div class='line' id='LC608'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">resized</span> <span class="o">=</span> <span class="nb">true</span><span class="p">;</span></div><div class='line' id='LC609'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC610'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="k">if</span> <span class="p">(</span><span class="n">resized</span><span class="p">)</span></div><div class='line' id='LC611'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">{</span></div><div class='line' id='LC612'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">cv</span><span class="o">::</span><span class="n">Size</span> <span class="n">vcSize</span><span class="p">(</span><span class="n">imgW</span><span class="p">,</span> <span class="n">imgH</span><span class="p">);</span></div><div class='line' id='LC613'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">cv</span><span class="o">::</span><span class="n">Mat</span> <span class="n">resizedImg</span><span class="p">(</span><span class="n">vcSize</span><span class="p">,</span> <span class="n">CV_8UC3</span><span class="p">);</span></div><div class='line' id='LC614'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">cv</span><span class="o">::</span><span class="n">resize</span><span class="p">(</span><span class="n">mRefImage</span><span class="p">,</span> <span class="n">resizedImg</span><span class="p">,</span> <span class="n">vcSize</span><span class="p">);</span></div><div class='line' id='LC615'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRefImage</span><span class="p">.</span><span class="n">release</span><span class="p">();</span></div><div class='line' id='LC616'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="n">mRefImage</span> <span class="o">=</span> <span class="n">resizedImg</span><span class="p">.</span><span class="n">clone</span><span class="p">();</span></div><div class='line' id='LC617'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC618'>&nbsp;&nbsp;&nbsp;&nbsp;<span class="p">}</span></div><div class='line' id='LC619'><span class="p">}</span></div></pre></div></td>
          </tr>
        </table>
  </div>

  </div>
</div>

<a href="#jump-to-line" rel="facebox[.linejump]" data-hotkey="l" class="js-jump-to-line" style="display:none">Jump to Line</a>
<div id="jump-to-line" style="display:none">
  <form accept-charset="UTF-8" class="js-jump-to-line-form">
    <input class="linejump-input js-jump-to-line-field" type="text" placeholder="Jump to line&hellip;" autofocus>
    <button type="submit" class="button">Go</button>
  </form>
</div>

        </div>

      </div><!-- /.repo-container -->
      <div class="modal-backdrop"></div>
    </div><!-- /.container -->
  </div><!-- /.site -->


    </div><!-- /.wrapper -->

      <div class="container">
  <div class="site-footer">
    <ul class="site-footer-links right">
      <li><a href="https://status.github.com/">Status</a></li>
      <li><a href="http://developer.github.com">API</a></li>
      <li><a href="http://training.github.com">Training</a></li>
      <li><a href="http://shop.github.com">Shop</a></li>
      <li><a href="/blog">Blog</a></li>
      <li><a href="/about">About</a></li>

    </ul>

    <a href="/">
      <span class="mega-octicon octicon-mark-github" title="GitHub"></span>
    </a>

    <ul class="site-footer-links">
      <li>&copy; 2014 <span title="0.17594s from github-fe120-cp1-prd.iad.github.net">GitHub</span>, Inc.</li>
        <li><a href="/site/terms">Terms</a></li>
        <li><a href="/site/privacy">Privacy</a></li>
        <li><a href="/security">Security</a></li>
        <li><a href="/contact">Contact</a></li>
    </ul>
  </div><!-- /.site-footer -->
</div><!-- /.container -->


    <div class="fullscreen-overlay js-fullscreen-overlay" id="fullscreen_overlay">
  <div class="fullscreen-container js-fullscreen-container">
    <div class="textarea-wrap">
      <textarea name="fullscreen-contents" id="fullscreen-contents" class="fullscreen-contents js-fullscreen-contents" placeholder="" data-suggester="fullscreen_suggester"></textarea>
    </div>
  </div>
  <div class="fullscreen-sidebar">
    <a href="#" class="exit-fullscreen js-exit-fullscreen tooltipped tooltipped-w" aria-label="Exit Zen Mode">
      <span class="mega-octicon octicon-screen-normal"></span>
    </a>
    <a href="#" class="theme-switcher js-theme-switcher tooltipped tooltipped-w"
      aria-label="Switch themes">
      <span class="octicon octicon-color-mode"></span>
    </a>
  </div>
</div>



    <div id="ajax-error-message" class="flash flash-error">
      <span class="octicon octicon-alert"></span>
      <a href="#" class="octicon octicon-remove-close close js-ajax-error-dismiss"></a>
      Something went wrong with that request. Please try again.
    </div>


      <script crossorigin="anonymous" src="https://assets-cdn.github.com/assets/frameworks-ab437d541e9c517df422126b73a5694a1d43ee9e.js" type="text/javascript"></script>
      <script async="async" crossorigin="anonymous" src="https://assets-cdn.github.com/assets/github-f8fc00b8934006933bc2391fd76f435ac85a7016.js" type="text/javascript"></script>
      
      
  </body>
</html>

