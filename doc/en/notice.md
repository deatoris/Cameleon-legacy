<div class="post full-width clearfix">
<p><div><a href="#introduction">Introduction</a></div>
<div><a href="#install">Install &#038; Launch</a></div>
<div><a href="#instance">Instance</a></div>
<div><a href="#Composer">Composer</a></div>
<div><a href="#OperatorConnector">Operator &#038; Connector</a></div>
<div><a href="#Controls">Control</a></div>
<div><a href="#SubComposition">Sub process &#038; Patterns</a></div>
<div><a href="#TextShape">Text &#038; Shape</a></div>
<div><a href="#Navigation">Navigation</a></div>
<div><a href="#Controller">Controller</a></div>
<div><a href="#Project">Project</a></div>
<div><a href="#Properties">Properties</a></div>
<div><a href="#Error">Error</a></div>
<div><a href="#CLI">Command Line Interface</a></div>
<p></br></br></p>

<p><a name="introduction"></a><br />
<strong><br />
<h2>Introduction</h2>
<p style="text-align: right;"><a href="#">back to top</a></p>	

<p></strong><br />
<a href="./concepts.md">Concepts article</a> is a good start to understand Caméléon underlying concepts.<br />
</p>
<div align="center">
<a href="img/Navigation001.jpg"><img src="img/Navigation001-300x225.jpg" alt="" title="Navigation001" width="300" height="225" class="alignnone size-medium wp-image-855" /></a><br />
<em>Figure 1 &#8211; A composition</em><br />
</div>
</p>
<p>Caméléon is a graphical algorithm editor. You can combine elementary components to create new compositions [Fig. 1]. All component construction features are based on a drag &#038; drop mechanism.</p>
<p>Caméléon allow the creation of those components:<br />
* <strong>Operator</strong>: Like a mathematical function, an operator, f, produces output data, y, from input data, x, as follows y = f(x). An operator (big center circle) has some input connector containing input data (little black circles including little colored or gray circles) and some output connector containing output data (little colored or gray circles including black circles).<br />
* <strong>Control</strong>: A control is an elementary component to manipulate, set or view datas. It can be connected to operators inputs &#038; outputs.<br />
* <strong>Pattern</strong>: A pattern is a set of operators &#038; controls aggregate in an elementary component. From a part of a composition you can create a sub composition, a sub process. A sub composition is reusable &#038; sharable as pattern.<br />
* <strong>Text &#038; Shape</strong>: Text &#038; Shape are used to structure &#038; comment compositions.</p>



<p><a name="install"></a></p>
<h2><strong>Install &#038; Launch</strong></h2>
If your want to install a pre-build version:<br/>
* Follow this link to <a href="./install_windows.md">install Caméléon on windows 7 à 10</a>,<br />
* Follow this link to <a href="./install_debian.md">install Caméléon on debian</a>.<br />
<br />
If your want to build your own version:<br/>
* Follow this link to <a href="./build_debian.md">install Caméléon on windows 7 à 10</a>,<br />
* Follow this link to <a href="./build_windows.md">build Caméléon on linux debian</a>.<br />




<p><a name="instance"></a><br />
<strong><br />
<h2>Instance</h2>
<p style="text-align: right;"><a href="#">back to top</a></p>	
<p></strong><br />
A Caméléon instance is a directory holding all files, binaries needed to execute Caméléon Editor.<br />
In the Caméléon, some directories and files are available:</p>
<pre>
/log
/ressource
/pattern
/project
instance.conf
[on linux] cameleon
[on windows] cameleon.exe
</pre>
<p>The root directory contains all binaries to execute Caméléon.<br />
The log directory contains all Caméléon instance log files.<br />
The ressource directory contains all ressource file needed by Caméléon (images &#8230;).<br />
The pattern directory is the directory where patterns are stored by Caméléon Creator.<br />
The project directory contains example projects. This directory must be used to store your projects.<br />
The instance.conf file is an XML configuration file for Caméléon.<br />



<p><a name="Composer"></a><br />
<strong><br />
<h2>Composer</h2>
<p style="text-align: right;"><a href="#">back to top</a></p>	
<p></strong><br />
The composer allows to create and connect graphically elementary components (operator, controls, patterns) to build new composition.<br />
<div align="center">
<a href="img/Composer.jpg"><img src="img/Composer-300x171.jpg" alt="" title="Composer" width="500" class="alignnone size-medium wp-image-829" /></a><br />
<em>Figure 2 &#8211; The composer</em><br />
</div></p>
<p>
* <strong>Menu Bar</strong> [Fig. 3]: The menu bar gives you access to management features<br />
* <strong>Player Bar</strong> [Fig. 5]: Start, pause &#038; stop the engine. You can either launch the engine step by step.<br />
* <strong>Mode Bar</strong> [Fig. 6]: Three editing modes are available; composition (arrow icon), shape (square icon) &#038; text (A icon).<br />
* <strong>Comment Bar</strong> [Fig. 7]: Set up text &#038; shape style &#038; color.<br />
* <strong>Glossary Tree</strong> [Fig. 4]: The glossary is like a component repository. You can create component from here.<br />
* <strong>Project Tree</strong>: The project tree expose your prototype structure. You can navigate through sub processes &#038; interfaces. You can add or delete interfaces.<br />
* <strong>Composition View</strong>: The black center square. This is the place to create operators, controls &#038; set up compositions.<br />
* <strong>Caméléon Outputs</strong>: At the bottom, two tabs are available. The first, consol outputs, shows all Caméléon outputs. To set console outputs, go to windows > properties > project  > Consol Outputs, set outputs to enable &#038; select your outputs level for each logs. The second, Executions problems, shows all execution problmes actualy avaible. By clicking on them, the system highlight the erroneous component.</p>


<h3 style="text-align: left;">Menu</h3>
<p>Menu gives access to saved file manager, properties etc.<br />
<div align="center">
<a href="img/Composer_Menu.jpg"><img src="img/Composer_Menu.jpg" alt="" title="Composer_Menu" width="254" height="26" class="alignnone size-full wp-image-831" /></a><br />
Figure 3 &#8211; Menu Bar<br />
</div>
<p>
* <strong>File</strong>: Save &#038; Load Caméléon *.cm files.<br />
* <strong>Windows</strong>: Hide &#038; Show windows.<br />
* <strong>Properties</strong>: Go to project or instance properties dialogs.<br />
* <strong>Help</strong>: Go to Caméléon web site to find information.</p>


<h3 style="text-align: left;">Glossary</h3>
<p>The glossary gives access to all components which can be created by Caméléon Creator.<br />
<div align="center">
<a href="img/Composer_Glossary.jpg"><img src="img/Composer_Glossary.jpg" alt="" title="Composer_Glossary" width="268" height="562" class="alignnone size-full wp-image-830" /></a><br />
Figure 4 &#8211; Glossary<br />
</div>
</p>
<p>The glossary is a tree list view which exposed all components which can be created.<br />
The glossary is devided in three parts:<br />
* <strong><a href="concepts.html#operator">Operator</a></strong>: Set of all available algorithms. Directories under the Operator directory are named glossary.<br />
* <strong><a href="concepts.html#control">Control</a></strong>: Set of all available controls. Directories under the Control directory are named glossary.<br />
* <strong><a href="concepts.html#pattern">Pattern</a></strong>: Set of all available user created patterns. </p>
<p><strong>Browse the glossary</strong>:<br />
1- Open a component directory,<br />
2- Little <strong>O</strong> icons identify components which can be created. Drag &#038; Drop it to the composition view to create a component. </p>
<p><strong>Search an operator</strong>:<br />
1- Click on the search edit box,<br />
2- Write a key word to search,<br />
3- The search tool highlight corresponding components.</p>


<h3 style="text-align: left;">Player Bar</h3>
<p>The player bar allow you to launch or stop the Caméléon engine.<br />
<div align="center">
<a href="img/Composer_Player_Bar.jpg"><img src="img/Composer_Player_Bar.jpg" alt="" title="Composer_Player_Bar" width="161" height="41" class="alignnone size-full wp-image-832" /></a><br />
Figure 5 &#8211; Player Bar<br />
</div>
</p>
<p>
* <strong>Run</strong>: to launch the composition execution,<br />
* <strong>Pause</strong>: to pause the execution,<br />
* <strong>Stop</strong>: to stop the execution,<br />
* <strong>Next step</strong>: in pause mode, allow you to process one next operator.</p>


<h3 style="text-align: left;">Mode Bar</h3>
<p>The mode bar allows to change the edition mode.<br />
<div align="center">
<a href="img/Edit_Mode.jpg"><img src="img/Edit_Mode.jpg" alt="" title="Edit_Mode" width="116" height="41" class="alignnone size-full wp-image-846" /></a><br />
Figure 6 &#8211; Mode Bar<br />
</div>
</p>
<p>
* <strong>Composition Mode</strong>: The mode to compose with operators &#038; controls,<br />
* <strong>Shape Mode</strong>: The mode to draw shapes,<br />
* <strong>Text Mode</strong>: The mode to write text (comments &#8230;).</p>


<h3 style="text-align: left;">Comment Bar</h3>
<p>The comment bar allow to manage text style &#038; shape colors.<br />
<div align="center">
<a href="img/Editor_Options.jpg"><img src="img/Editor_Options.jpg" alt="" title="Editor_Options" width="441" height="40" class="alignnone size-full wp-image-847" /></a><br />
Figure 7 &#8211; Comment Bar<br />
</div>
</p>
<p>Those actions has effect on selected texts. Multiple selection is allowed.<br />
* <strong>Police selector</strong>: Change selected text police,<br />
* <strong>Size selector</strong>: Change selected text size,<br />
* <strong>Bold button</strong>: Change selected text to Bold,<br />
* <strong>Italic button</strong>: Change selected text to italic,<br />
* <strong>Underline button</strong>: Underline selected text,</p>
<p>This action has effect on selected texts &#038; shape. Multiple selection is allowed.<br />
* Color selector: Change selected text or shape color.</p>


<h3 style="text-align: left;">Bring &#038; Send</h3>
<p>The bring &#038; send bar allows to manage component Z axis.<br />
<div align="center">
<a href="img/Up_Down.jpg"><img src="img/Up_Down.jpg" alt="" title="Up_Down" width="93" height="40" class="alignnone size-full wp-image-873" /></a><br />
Figure 8 &#8211; Bring &#038; Send<br />
</div>
</p>
<p>Those actions has effect on selected components. Multiple selection is allowed.<br />
* <strong>Bring to front</strong>: Bring selected components to front,<br />
* <strong>Send to back</strong>: Send selected components to back.</p>

<h3 style="text-align: left;">File Menu</h3>
<p>File menu allows to manage load &#038; save project.<br />
<div align="center">
<a href="img/Menu_File.jpg"><img src="img/Menu_File.jpg" alt="" title="Menu_File" width="262" height="193" class="alignnone size-full wp-image-850" /></a><br />
Figure 12 &#8211; File Menu<br />
</div>
</p>
<p>
* <strong>new Project &#8230;</strong>: Create a new project,<br />
* <strong>open recent project</strong>: Open a recent project list. Select a project to open it,<br />
* <strong>open project &#8230;</strong>: Open a project on your computer,<br />
* <strong>save project</strong>: Save a project on your computer,<br />
* <strong>save project as &#8230;</strong>: Save a project in a specific directory,<br />
* <strong>exit</strong>: Close Caméléon,<br />
* <strong>save and restart</strong>: Save and restart Caméléon.</p>

<h3 style="text-align: left;">Windows Menu</h3>
<p>The windows menu allows to manage windows visibility.<br />

<div align="center">
<a href="img/Menu_Windows.jpg"><img src="img/Menu_Windows.jpg" alt="" title="Menu_Windows" width="178" height="107" class="alignnone size-full wp-image-853" /></a><br />
Figure 9 &#8211; Windows Menu<br />
</div>

</p>
<p>
* <strong>controller</strong>: Show/Hide the controller window,<br />
* <strong>project tree</strong>: Show/Hide project tree,<br />
* <strong>console output</strong>: Show/Hide console outputs,<br />
* <strong>glossary tree</strong>: Show/Hide glossary tree.</p>


<h3 style="text-align: left;">Properties Menu</h3>
<p>The properties menu allows to manage instance &#038; project properties.<br />

<div align="center">
<a href="img/Menu_Properties.jpg"><img src="img/Menu_Properties.jpg" alt="" title="Menu_Properties" width="140" height="54" class="alignnone size-full wp-image-852" /></a><br />
Figure 10 &#8211; Properties Menu<br />
</div>

</p>
<p>
* <strong>cameleon</strong>: Show Caméléon properties dialog,<br />
* <strong>project</strong>: Show project properties dialog.</p>


<h3 style="text-align: left;">Help Menu</h3>
<p>Help menu gives access to support &#038; documentation web links.<br />
<div align="center">
<a href="img/Menu_Help.jpg"><img src="img/Menu_Help.jpg" alt="" title="Menu_Help" width="185" height="100" class="alignnone size-full wp-image-851" /></a><br />
Figure 11 &#8211; Help Menu<br />
</div>
</p>
<p>
* <strong>documentation &#8230;</strong>: Open online documentation in a web browser,<br />
* <strong>community &#8230;</strong>: Open community links in a web browser,<br />
* <strong>report a bug &#8230;</strong>: Open the online contact form,<br />
* <strong>about</strong>: Open the about dialog (Caméléon version &#038; licence are available from here).</p>
<p><a name="OperatorConnector"></a><br />
<strong><br />
<h2>Operator &#038; Connector</h2>
<p style="text-align: right;"><a href="#">back to top</a></p>	
<p></strong></p>


<h3 style="text-align: left;">Create &#038; move an operator</h3>
<p>
<div align="center">
<a href="img/Operator000.jpg"><img src="img/Operator000-286x300.jpg" alt="" title="Operator000" width="286" height="300" class="alignnone size-medium wp-image-859" /></a><br />
Figure 12 &#8211; Create an operator<br />
</div>
</p>
<p>1- Identify the operator you want to create by browsing or searching the glossary,<br />
2- Operators are available behind the &#8220;operator&#8221; directory and are identified by a <strong>O</strong> icon,<br />
3- Drag the operator&#8217;s name from the glossay and drop it in the composition view,<br />
4- It creates an operator at the dropping position,<br />
5- You can drag &#038; drop it on the composition view to change its position (also works in multi selection).</p>


<h3 style="text-align: left;">Operator &#038; Connector Statuses</h3>
<p>The operator status gives information about its execution.<br />
<div align="center">
<a href="img/notrunnable.jpg"><img src="img/notrunnable-300x150.jpg" alt="" title="notrunnable" width="200" class="alignnone size-medium wp-image-978" /></a><a href="img/runnable.jpg"><img src="img/runnable-300x150.jpg" alt="" title="runnable" width="200" class="alignnone size-medium wp-image-979" /></a><a href="img/running.jpg"><img src="img/running-300x150.jpg" alt="" title="running" width="200" class="alignnone size-medium wp-image-980" /></a><a href="img/error.jpg"><img src="img/error-300x150.jpg" alt="" title="error" width="200" class="alignnone size-medium wp-image-981" /></a><br />
Figure 19 &#8211; Operator statuses<br />
</div>
<br />
* <strong>Runnable</strong>: Green color. The operator can be executed.<br />
* <strong>Running</strong>: Yellow color. The operator is being executed.<br />
* <strong>Not Runnable</strong>: Gray color. The operator can not be executed by the CVM.<br />
* <strong>Error</strong>: red color. An error occurred on this operator.</p>
<p>Connector status gives information about the connector&#8217;s data.<br />
<div align="center">
<a href="img/Empty.jpg"><img src="img/Empty.jpg" alt="" title="Empty" width="200" class="alignnone size-full wp-image-985" /></a><a href="img/New.jpg"><img src="img/New.jpg" alt="" title="New" width="200" class="alignnone size-full wp-image-986" /></a><a href="img/Old.jpg"><img src="img/Old.jpg" alt="" title="Old" width="200" class="alignnone size-full wp-image-987" /></a><br />
Figure 20 &#8211; Connector statuses<br />
</div><br />
* <strong>Empty</strong>: Gray color. No data available in the connector.<br />
* <strong>New</strong>: Green color. A new data is available in the connector.<br />
* <strong>Old</strong>: Blue color. An old data -means already used by an operator, is available in the connector.</p>


<h3 style="text-align: left;">Change connector position</h3>
<p>Organize simply the composition changing connector&#8217;s position.<br />
<div align="center">
<a href="img/Operator001.jpg"><img src="img/Operator001-300x215.jpg" alt="" title="Operator001" width="300" height="215" class="alignnone size-medium wp-image-860" /></a><a href="img/Operator002.jpg"><img src="img/Operator002-300x215.jpg" alt="" title="Operator002" width="300" height="215" class="alignnone size-medium wp-image-861" /></a><a href="img/Operator003.jpg"><img src="img/Operator003-300x215.jpg" alt="" title="Operator003" width="300" height="215" class="alignnone size-medium wp-image-862" /></a><br />
Figure 13 &#8211; Change connector position<br />
</div>
</p>
<p>1- Drag a connector to another position,<br />
2- Be careful to keep your mouse into the connector circle,<br />
3- Drop the mouse to set the connector position.</p>


<h3 style="text-align: left;">Show operator information</h3>
<p>
<div align="center">
<a href="img/Operator004.jpg"><img src="img/Operator004-300x110.jpg" alt="" title="Operator004" width="300" height="110" class="alignnone size-medium wp-image-863" /></a><a href="img/Operator005.jpg"><img src="img/Operator005-300x110.jpg" alt="" title="Operator005" width="300" height="110" class="alignnone size-medium wp-image-864" /></a><a href="img/connector_tooltip.jpg"><img src="img/connector_tooltip-300x110.jpg" alt="" title="connector_tooltip" width="300" height="110" class="alignnone size-medium wp-image-991" /></a><br />
Figure 14 &#8211; Show operator information<br />
</div></p>
<p>Hoover your mouse on an operator or a connector to show information. Select one or more component, press the &#8216;W&#8217; key to show more informations.</p>


<h3 style="text-align: left;">Operator Menu</h3>
<p>Right Click on an operator to show the menu.<br />
<div align="center">
<a href="img/Operator_Menu.jpg"><img src="img/Operator_Menu.jpg" alt="" title="Operator_Menu" width="179" height="74" class="alignnone size-full wp-image-857" /></a><br />
Figure 15 &#8211; Operator Menu<br />
</div>
</p>
<p>
* <strong>show infos</strong>: As &#8216;W&#8217; key press, it shows operator description,<br />
* <strong>delete</strong>: delete the operator.<br />
* <strong>(optional) actions</strong>: can expose operator specific actions.</p>


<h3 style="text-align: left;">Connection</h3>
<p>
<div align="center">
<a href="img/Connection001.jpg"><img src="img/Connection001-300x235.jpg" alt="" title="Connection001" width="300" height="235" class="alignnone size-medium wp-image-833" /></a><a href="img/Connection002.jpg"><img src="img/Connection002-300x235.jpg" alt="" title="Connection002" width="300" height="235" class="alignnone size-medium wp-image-834" /></a><a href="img/Connection003.jpg"><img src="img/Connection003-300x235.jpg" alt="" title="Connection003" width="300" height="235" class="alignnone size-medium wp-image-835" /></a><br />
Figure 18 &#8211; Connection<br />
</div>
</p>
<p>1- Drag a connector from its position to somewhere on the composition view,<br />
2- It shows compatible connectors (identified with yellow circles),<br />
3- Drop on a compatible connector to connect.</p>
<p>After a connection, when a data will be available on output connector, it will be automatically dumped into input connector.</p>
<p><strong>Note</strong>: Some datas are shared by reference or by value. The next 2.0.8 version will expose this characteristic more clearly.</p>


<h3 style="text-align: left;">Connector Menu</h3>
<p>Right click on a connector to show the connector contextual menu.<br />
<div align="center">
<a href="img/Connector_Menu.jpg"><img src="img/Connector_Menu.jpg" alt="" title="Connector_Menu" width="197" height="81" class="alignnone size-full wp-image-995" /></a><br />
Figure 21 &#8211; Connector Menu<br />
</div></p>
<p>
* <strong>disconnect</strong>: disconnect the connector,<br />
* <strong>disconnect control</strong>: disconnect the connector from a control connector,<br />
* <strong>show control</strong>: show the control connector connected to this connector.</p>


<h3 style="text-align: left;">Multi Selection</h3>
<p><div align="center">
<a href="img/MultiSelect.jpg"><img src="img/MultiSelect-300x225.jpg" alt="" title="MultiSelect" width="300" height="225" class="alignnone size-medium wp-image-923" /></a><br />
Figure 16 &#8211; Multi Selection<br />
</div>
</p>
<p>1- Drag the mouse from an empty space on the composition view to somewhere,<br />
2- It draws a selection square,<br />
3- Drop somewhere,<br />
4- It selects components which are under the selection square.</p>
<p>After multi-selection, click right somewhee to show the multi-selection contextual menu.</p>
<p>
<div align="center">
<a href="img/Operator_MultiSelection_Menu.jpg"><img src="img/Operator_MultiSelection_Menu.jpg" alt="" title="Operator_MultiSelection_Menu" width="253" height="93" class="alignnone size-full wp-image-858" /></a><br />
Figure 17 &#8211; Multi Selection Menu<br />
</div>
</p>
<p>
* <strong>show infos</strong>: show information about selected components,<br />
* <strong>merge into sub process</strong>: merge selected component into a new sub process,<br />
* <strong>delete</strong>: delete selected components.</p>
<p><a name="Controls"></a><br />
<strong><br />




<h2>Controls</h2>
<p><p style="text-align: right;"><a href="#">back to top</a></p>	
</strong></p>


<h3 style="text-align: left;">Create control</h3>

<p>
<div align="center">
<a href="img/Control000.jpg"><img src="img/Control000-286x300.jpg" alt="" title="Control000" width="286" height="300" class="alignnone size-medium wp-image-842" /></a><br />
Figure 22 &#8211; Create control<br />
</div>
</p>
<p>1- Identify the control you want to create by browsing or searching the glossary,<br />
2- Controls are available behind the &#8220;control&#8221; directory and are identified by a <strong>O</strong> icon,<br />
3- Drag the control&#8217;s name from the glossay and drop it in the composition view,<br />
4- It creates a control at the dropping position,<br />
5- You can drag &#038; drop it on the composition view to change its position (also works in multi selection).</p>

<p>
<div align="center">
<a href="img/Control001.jpg"><img src="img/Control001.jpg" alt="" title="Control001" width="235" height="146" class="alignnone size-full wp-image-843" /></a><a href="img/Control002.jpg"><img src="img/Control002.jpg" alt="" title="Control002" width="235" height="146" class="alignnone size-full wp-image-1005" /></a><a href="img/Control003.jpg"><img src="img/Control003.jpg" alt="" title="Control003" width="235" height="146" class="alignnone size-full wp-image-1006" /></a><br />
Figure 23 &#8211; Different kind of controls<br />
</div>
</p>
<p>Some controls are just data view, other can just edit &#038; set data (often by pressing enter after edition). An other kind of control can edit datas using inputs datas.</p>
<p>You can resize the control by drag &#038; dropping the little bottom right gray circle.</p>


<h3 style="text-align: left;">Control Menu</h3>
<p>
<div align="center">
<a href="img/control_composer_menu.jpg"><img src="img/control_composer_menu.jpg" alt="" title="control_composer_menu" width="160" height="27" class="alignnone size-full wp-image-993" /></a><br />
Figure 25 &#8211; Control Menu<br />
</div>
</p>
<p>Delete selected controls.</p>


<h3 style="text-align: left;">Control Connection</h3>
<p><div align="center">
<a href="img/Control_Connection001.jpg"><img src="img/Control_Connection001-300x130.jpg" alt="" title="Control_Connection001" width="300" height="130" class="alignnone size-medium wp-image-836" /></a><a href="img/Control_Connection002.jpg"><img src="img/Control_Connection002-300x130.jpg" alt="" title="Control_Connection002" width="300" height="130" class="alignnone size-medium wp-image-837" /></a><br />
Figure 24 &#8211; Control connection<br />
</div>
</p>
<p>1- Click on a control connector,<br />
2- See highlighted compatible connectors,<br />
3- Click on one compatible connectors to connect,<br />
4- Connector connected to control are identified with a white circle.</p>


<h3 style="text-align: left;">Show control Connection</h3>
<p><div align="center">
<a href="img/Control_Connection003.jpg"><img src="img/Control_Connection003-300x130.jpg" alt="" title="Control_Connection003" height="110" class="alignnone size-medium wp-image-838" /></a><a href="img/Control_Connection004.jpg"><img src="img/Control_Connection004-300x130.jpg" alt="" title="Control_Connection004" height="110" class="alignnone size-medium wp-image-839" /></a><a href="img/Control_Connection005.jpg"><img src="img/Control_Connection005-300x116.jpg" alt="" title="Control_Connection005" height="110" class="alignnone size-medium wp-image-840" /></a><br />
Figure 26 &#8211; Show connection<br />
</div></p>
<p>1- Double click on a connector connected to a control,<br />
2- It highlights the connected connector.</p>
<p>Outputs operator connector can be connected to one operator connector and one control connector. In this case, the output datas can be used by an operator and, also, be viewed by a control.</p>
<p><strong>Note</strong>: Be careful to data by reference.</p>
<p><a name="SubComposition"></a><br />
<strong><br />
<h2>Sub-Composition</h2>
<p><p style="text-align: right;"><a href="#">back to top</a></p>	
</strong></p>


<h3 style="text-align: left;">Create a sub composition</h3>

<p>
<div align="center">
<a href="img/sub-process001.jpg"><img src="img/sub-process001-300x110.jpg" alt="" title="sub-process001" width="300" height="110" class="alignnone size-medium wp-image-870" /></a><br />
Figure 27 &#8211; A sub composition<br />
</div>
</p>

<p>1- Select more than one component (operator, control, text or shape),<br />
2- Click left somewhere,<br />
3- A multi selection menu appears,<br />
3- Click on &#8220;merge into sub process&#8221;,<br />
4- It creates a sub process, a gray circle.</p>


<h3 style="text-align: left;">Browse sub process</h3>
<p><div align="center">
<a href="img/sub-process002.jpg"><img src="img/sub-process002-300x256.jpg" alt="" title="sub-process002" width="300" height="256" class="alignnone size-medium wp-image-871" /></a><br />
Figure 28 &#8211; Browse a sub process<br />
</div></p>
<p>1- Double click on a sub process to get into,<br />
2- Double click on the sub process background to go to the upscale level.</p>


<h3 style="text-align: left;">Expose a connector</h3>
<p><div align="center">
<a href="img/Expose001.jpg"><img src="img/Expose001-300x289.jpg" alt="" title="Expose001" width="300" height="289" class="alignnone size-medium wp-image-1017" /></a><a href="img/Expose002.jpg"><img src="img/Expose002-300x289.jpg" alt="" title="Expose002" width="300" height="289" class="alignnone size-medium wp-image-1018" /></a><a href="img/Expose004.jpg"><img src="img/Expose004-300x289.jpg" alt="" title="Expose004" width="300" height="289" class="alignnone size-medium wp-image-1019" /></a><br />
Figure 29 &#8211; Expose a connector<br />
</div></p>
<p>1- Drag a connector,<br />
2- See highlighted compatible connectors and the sub composition highlighted,<br />
3- Drop your mouse on the sub composition external background,<br />
4- It create a new connector bridge on the sub composition and connect the connector to it.</p>


<h3 style="text-align: left;">Expose a connector (alternative)</h3>
<p><div align="center">
<a href="img/Expose0011.jpg"><img src="img/Expose0011-300x289.jpg" alt="" title="Expose001" width="300" height="289" class="alignnone size-medium wp-image-1020" /></a><a href="img/Expose003.jpg"><img src="img/Expose003-300x289.jpg" alt="" title="Expose003" width="300" height="289" class="alignnone size-medium wp-image-1021" /></a><br />
Figure 55 &#8211; Expose a connector (alternative)<br />
</div></p>
<p>1- In a sub composition, drag a connector,<br />
2- Drop it on a sub composition connector,<br />
3- Connectors are connected.</p>


<h3 style="text-align: left;">Connect to sub process</h3>
<p><div align="center">
<a href="img/connectto001.jpg"><img src="img/connectto001-300x289.jpg" alt="" title="connectto001" width="300" height="289" class="alignnone size-medium wp-image-1022" /></a><a href="img/connectto003.jpg"><img src="img/connectto003-300x289.jpg" alt="" title="connectto003" width="300" height="289" class="alignnone size-medium wp-image-1023" /></a><br />
Figure 56 &#8211; Connect to sub process<br />
</div></p>
<p>1- At upper scale, drag a connector,<br />
2- Drop the mouse on a sub composition,<br />
3- It create a new sub composition bridge &#038; connect the dropped connector to it.</p>


<h3 style="text-align: left;">Sub Process Menu</h3>
<p>Click right on a sub composition to show the sub composition Menu.<br />
<div align="center">
<a href="img/sub-process_Menu1.jpg"><img src="img/sub-process_Menu1.jpg" alt="" title="sub-process_Menu" width="211" height="149" class="alignnone size-full wp-image-1047" /></a><br />
Figure 30 &#8211; Sub Process Menu<br />
</div></p>
<p>
* <strong>show infos</strong>: Show information about the sub process (those information are editable through the properties dialog),<br />
* <strong>save as a pattern</strong>: Save this sub composition as a reusable pattern (see the pattern directory in the glossary),<br />
* <strong>properties &#8230;</strong>: Show the properties dialog,<br />
* <strong>delete</strong>: delete the sub composition,<br />
* <strong>unmerge</strong>: restore the composition without the sub process.</p>


<h3 style="text-align: left;">Sub Process Properties</h3>
<p><div align="center">
<a href="img/sub-process-properties.jpg"><img src="img/sub-process-properties-300x212.jpg" alt="" title="sub-process-properties" width="300" height="212" class="alignnone size-medium wp-image-872" /></a><br />
Figure 31 &#8211; Sub Process Properties Dialog<br />
</div></p>
<p>
* <strong>First edit box</strong>: This is the sub composition name field. You can edit the name here,<br />
* <strong>Second edit box</strong>: This is a description field. You can write the sub composition description here,<br />
* <strong>Connector List</strong>: This is the list of all sub composition connectors. The name field is editable,<br />
* <strong>Apply button</strong>: Click on this button to apply your change,<br />
* <strong>Cancel button</strong>: Click on this button to discard your change.</p>


<h3 style="text-align: left;">Save as a pattern</h3>
<p><div align="center">
<a href="img/patterns.jpg"><img src="img/patterns.jpg" alt="" title="patterns" width="277" height="185" class="alignnone size-full wp-image-1037" /></a><br />
Figure 57 &#8211; Save as a pattern<br />
</div><br />
When you click on save as pattern on the sub composition menu, it create a new entry under the &#8220;pattern&#8221; directory in the glossary.</p>


<h3 style="text-align: left;">Create a pattern</h3>
<p><div align="center">
<a href="img/create_pattern.jpg"><img src="img/create_pattern-300x178.jpg" alt="" title="create_pattern" width="300" height="178" class="alignnone size-medium wp-image-1038" /></a><br />
Figure 35 &#8211; Create a pattern<br />
</div></p>
<p>1- Identify the pattern you want to create by browsing or searching the glossary,<br />
2- Operators are available behind the &#8220;pattern&#8221; directory and are identified by a <strong>O</strong> icon,<br />
3- Drag the pattern&#8217;s name from the glossay and drop it in the composition view,<br />
4- It creates a sub composition at the dropping position,<br />
5- You can drag &#038; drop it on the composition view to change its position (also works in multi selection).</p>
<p>Like for operator, you can change a connector&#8217;s position by drag &#038; dropping it.</p>


<h3 style="text-align: left;">Create a pattern directory</h3>
<p>Click right on the pattern directory or on an other directory under pattern and click on add directory. Select a name and it will create a new directory under the selected clicked right directory.</p>
<p>For sub directories or patterns,<br />
1- click right on a sub directory or a pattern,<br />
2- select rename menu to rename the directory or the pattern,<br />
3- select delete menu to delete the directory or the pattern.</p>


<h3 style="text-align: left;">Organize patterns</h3>
<p>Drag &#038; drop pattern names or directories to organize patterns.</p>
<p><a name="TextShape"></a><br />
<strong><br />



<h2>Text &#038; Shape</h2>
<p><p style="text-align: right;"><a href="#">back to top</a></p>	
</strong></p>


<h3 style="text-align: left;">Create a shape</h3>
<p><div align="center">
<a href="img/Shape.jpg"><img src="img/Shape-300x244.jpg" alt="" title="Shape" width="300" height="244" class="alignnone size-medium wp-image-925" /></a><br />
Figure 32 &#8211; Create a shape<br />
</div></p>
<p>1- Click on the shape mode button,<br />
2- Drag your mouse from somewhere on the composition view,<br />
3- It draws like a selection square,<br />
4- Drop your mouse to define the size of the shape,<br />
5- Click on the composition mode button,<br />
6- By drag &#038; drop, move the shape position,<br />
7- By drag &#038; dropping little circle on the bottom right, you can change the shape size.</p>


<h3 style="text-align: left;">Write a text</h3>
<p><div align="center">
<a href="img/Comment.jpg"><img src="img/Comment.jpg" alt="" title="Comment" width="176" height="93" class="alignnone size-full wp-image-926" /></a><br />
Figure 33 &#8211; Write a text<br />
</div></p>
<p>1- Click on the text mode button,<br />
2- Right click somewhere on the composition view,<br />
3- Write your text,<br />
4- Click on the composition mode button,<br />
6- By drag &#038; drop, move the text position,</p>


<h3 style="text-align: left;">Shape &#038; Text Menu</h3>
<p><div align="center">
<a href="img/control_composer_menu.jpg"><img src="img/control_composer_menu.jpg" alt="" title="control_composer_menu" width="160" height="27" class="alignnone size-full wp-image-993" /></a><br />
Figure 39 &#8211; Shape &#038; Text Menu<br />
</div></p>
<p>Delete selected shape and text.</p>
<p><a name="Navigation"></a><br />
<strong><br />




<h2>Navigation</h2>
<p><p style="text-align: right;"><a href="#">back to top</a></p>	
</strong><br />
<div align="center">
<a href="img/Navigation000.jpg"><img src="img/Navigation000-300x225.jpg" alt="" title="Navigation000" width="300" height="225" class="alignnone size-medium wp-image-854" /></a><a href="img/Navigation0011.jpg"><img src="img/Navigation0011-300x225.jpg" alt="" title="Navigation001" width="300" height="225" class="alignnone size-medium wp-image-883" /></a><a href="img/Navigation002.jpg"><img src="img/Navigation002-300x225.jpg" alt="" title="Navigation002" width="300" height="225" class="alignnone size-medium wp-image-856" /></a><br />
Figure 34 &#8211; Zoom in &#038; out<br />
</div></p>
<p>
* Hold the Q key on the keyboard and drag &#038; drop your mouse to change the composition view position,<br />
* Hold the Q key on the keyboard and use the wheel of the mouse to zoom in &#038; zoom out.</p>
<p><a name="Controller"></a><br />
<strong><br />




<h2>Controller</h2>
<p><p style="text-align: right;"><a href="#">back to top</a></p>	
</strong></p>
<p>The controller is used to create functional interface prototype usable without using the composer.</p>
<p><div align="center">
<a href="img/Controller.jpg"><img src="img/Controller-300x111.jpg" alt="" title="Controller" width="300" height="111" class="alignnone size-medium wp-image-844" /></a><br />
Figure 40 &#8211; Controller<br />
</div></p>
<p>
* <strong>Player Bar</strong> [Fig. 5]: Start, pause &#038; stop the engine. You can either launch the engine step by step.<br />
* <strong>Mode Bar</strong> [Fig. 6]: Three editing modes are available; composition (arrow icon), shape (square icon) &#038; text (A icon).<br />
* <strong>Comment Bar</strong> [Fig. 7]: Set up text &#038; shape style &#038; color.<br />
* <strong>Glossary Tree</strong> [Fig. 4]: The glossary is like a component repository. You can create component from here.<br />
* <strong>Project Tree</strong>: The project tree expose your prototype structure. You can navigate through sub processes &#038; interfaces. You can add or delete interfaces.<br />
* <strong>Interface View</strong>: The black center square. This is the place to create operators, controls &#038; set up compositions.</p>


<h3 style="text-align: left;">Controller&#8217;s glossary</h3>
<p><div align="center">
<a href="img/Controller_Glossary.jpg"><img src="img/Controller_Glossary-131x300.jpg" alt="" title="Controller_Glossary" width="131" height="300" class="alignnone size-medium wp-image-845" /></a><br />
Figure 40 &#8211; Controller&#8217;s glossary<br />
</div></p>
<p>The controller&#8217;s glossary is a tree list view which exposed all components which can be created with the controller.<br />
* <strong>Control</strong>: Set of all available controls.</p>


<h3 style="text-align: left;">Create a control</h3>
<p><div align="center">
Figure 42 &#8211; Create a control<br />
</div></p>
<p>1- Identify the control you want to create by browsing or searching the glossary,<br />
2- Controls are available behind the &#8220;control&#8221; directory and are identified by a <strong>O</strong> icon,<br />
3- Drag the control&#8217;s name from the glossay and drop it in the composition view,<br />
4- It creates a control at the dropping position,<br />
5- You can drag &#038; drop it on the composition view to change its position (also works in multi selection).</p>


<h3 style="text-align: left;">Control Menu</h3>
<p><div align="center">
<a href="img/control_controller_menu.jpg"><img src="img/control_controller_menu.jpg" alt="" title="control_controller_menu" width="167" height="82" class="alignnone size-full wp-image-994" /></a><br />
Figure 41 &#8211; Control Menu<br />
</div></p>
<p>
* persist: If active, the control will be available at the same position in all interface (see <a href="http://">create an interface</a>),<br />
* delete: Delete the control,<br />
* disconnect all: Disconnect all control connectors.</p>


<h3 style="text-align: left;">Connect a controller control to an operator</h3>
<p><div align="center">
<a href="img/controller_connection.jpg"><img src="img/controller_connection-300x191.jpg" alt="" title="controller_connection" width="300" height="191" class="alignnone size-medium wp-image-1034" /></a><a href="img/controller_connection002.jpg"><img src="img/controller_connection002-300x191.jpg" alt="" title="controller_connection002" width="300" height="191" class="alignnone size-medium wp-image-1035" /></a><br />
Figure 43 &#8211; Controller control connection<br />
</div></p>
<p>1- Click on a control connector on the controller window,<br />
2- See highlighted compatible connectors on the composer window,<br />
3- Click on one compatible connectors to connect,<br />
4- Connector connected to control are identified with a white circle.</p>
<p>Note: Be careful to find the right position &#038; size for both controller window &#038; composer window. We must see both the to composition view &#038; the interface view to connect correctly a control on the controller side to an operator on the composer side.</p>
<p>You can double click on the connected connector to see highlighted connected connector. </p>
<p><a name="Project"></a><br />
<strong><br />



<h2>Project</h2>
<p><p style="text-align: right;"><a href="#">back to top</a></p>	
</strong></p>
<p>The project tree (right part of the composer or the controller) can be shown or hided using menu <strong>windows </strong>> <strong>project tree</strong> action.</p>
<p><div align="center">
<a href="img/Project_Tree.jpg"><img src="img/Project_Tree-136x300.jpg" alt="" title="Project_Tree" width="136" height="300" class="alignnone size-medium wp-image-868" /></a><br />
Figure 44 &#8211; Project #1<br />
</div></p>
<p>The project tree allows to browse composer sub compositions or controller interfaces. It allows to create sub processes or interfaces.</p>


<h3 style="text-align: left;">Browse processes</h3>
<p><div align="center">
<a href="img/complex_project_tree.jpg"><img src="img/complex_project_tree.jpg" alt="" title="complex_project_tree" width="217" height="287" class="alignnone size-full wp-image-1045" /></a><br />
Figure 55 &#8211; Project #2<br />
</div></p>
<p>Double click on a process (<strong>O</strong> icons) to show the corresponding process on the composer.<br />
The top process (named &#8220;process&#8221;) goes to the main top composition.</p>


<h3 style="text-align: left;">Browse interfaces</h3>
<p>Double click on a process (<strong>Square green</strong> icons) to show the corresponding interface on the controller.<br />
The interface under pattern (named &#8220;creation&#8221;) goes to the main interface.</p>


<h3 style="text-align: left;">Create an interface</h3>
<p><div align="center">
<a href="img/project_icons.jpg"><img src="img/project_icons.jpg" alt="" title="project_icons" width="79" height="28" class="alignnone size-full wp-image-1042" /></a><br />
Figure 54 &#8211; Creation icons<br />
</div></p>
<p>Click on the <strong>square green plus</strong> icon to create a new interface. A pop-up to set up its name will appear.</p>


<h3 style="text-align: left;">Create a sub process</h3>
<p>Click on the <strong>blue/green graph node</strong> icon to create a new sub process. A pop-up to set up its name will appear.<br />
The sub composition is created at the center of the current composition/sub composition.</p>


<h3 style="text-align: left;">Project directory</h3>
<p>A project directory must be created by user for each new project. The save as feature must be used in this directory. For instance, we save as a new project named &#8220;project_name&#8221; in an empty project_name directory. The directory is composed of those sub-directories &#038; files:</p>
<pre>
project_name/log
project_name/ressource
project_name/target
project_name/tmp
project_name/project_name.cm
</pre>
<p>The log directory contain the cameleon.log file (and maybe others). In case of a crash, this file can be usefull to analyse the problem. Log output is configurable through the Project Properties Logs dialog.<br />
The ressource diretory is used to put all datas file used by the project. To share a project, it&#8217;s usefull to have a place to put those data sources (images, raw files, videos etc.).<br />
The target directory is <em>deprecated</em>.<br />
The tmp directory contain data file copy produced by the composition.<br />
The project_name.cm file is the XML file descriptor of the project (reference to operators, data, controls, interfaces &#038; sub compositions).</p>
<p><a name="Properties"></a><br />
<strong><br />



<h2>Properties</h2>
<p><p style="text-align: right;"><a href="#">back to top</a></p>	
</strong></p>


<h3 style="text-align: left;">Cameleon Properties Options</h3>
<p><div align="center">
<a href="img/Cameleon_Properties_Options.jpg"><img src="img/Cameleon_Properties_Options-300x144.jpg" alt="" title="Cameleon_Properties_Options" width="300" height="144" class="alignnone size-medium wp-image-828" /></a><br />
Figure 45 &#8211; Cameleon Properties Options<br />
</div></p>
<p>
* Instance path: The instance path,<br />
* Pattern path: Patterns are stored here,<br />
* Plugins path: deprecated,<br />
* Dictionaries path: dictionaries shared libraries are stored here,<br />
* Projects path: project must be saved here,<br />
* Ressources path: unstable,<br />
* Auto-save step number: Number of action to launch an auto-save.</p>


<h3 style="text-align: left;">Caméléon Properties Dictionaries</h3>
<p><div align="center">
<a href="img/Cameleon_Properties_Dico.jpg"><img src="img/Cameleon_Properties_Dico-300x144.jpg" alt="" title="Cameleon_Properties_Dico" width="300" height="144" class="alignnone size-medium wp-image-827" /></a><br />
Figure 46 &#8211; Caméléon Properties Dictionaries<br />
</div></p>
<p>Table:<br />
* name: The dictionary name,<br />
* path: The dictionary path,<br />
* version: (error) The version or an error message if miss loaded,<br />
* activated: enable or disable the dictionary (need a Caméléon restart).</p>


<h3 style="text-align: left;">Project Properties Options</h3>
<p><div align="center">
<a href="img/Project_Properties_Options.jpg"><img src="img/Project_Properties_Options-300x176.jpg" alt="" title="Project_Properties_Options" width="300" height="176" class="alignnone size-medium wp-image-866" /></a><br />
Figure 47 &#8211; Project Properties Options<br />
</div></p>
<p>
* project name<br />
* project version<br />
* tmp path<br />
* log path<br />
* projects path<br />
* ressources path<br />
* target directory<br />
* connectors behavior: debug tool<br />
* max threads: disabled because unstable</p>


<h3 style="text-align: left;">Project Readme</h3>
<p><div align="center">
<a href="img/Project_Properties_ReadMe.jpg"><img src="img/Project_Properties_ReadMe-300x178.jpg" alt="" title="Project_Properties_ReadMe" width="300" height="178" class="alignnone size-medium wp-image-867" /></a><br />
Figure 48 &#8211; Project Readme<br />
</div></p>
<p>The project readme editable text box.</p>


<h3 style="text-align: left;">Project Properties Logs</h3>
<p><div align="center">
<a href="img/Project_Properties_Logs.jpg"><img src="img/Project_Properties_Logs-300x178.jpg" alt="" title="Project_Properties_Logs" width="300" height="178" class="alignnone size-medium wp-image-865" /></a><br />
Figure 49 &#8211; Project Properties Logs<br />
</div></p>
<p>Firet section:<br />
* log enabled check box: Enable or disable all logs.</p>
<p>Table:<br />
* component name: Log component are autonomous log which focus on specifics parts of the Caméléon execution.<br />
* log level: You can select your log level or shutdown a log component.</p>


<h3 style="text-align: left;">Sub process properties</h3>
<p><div align="center">
<a href="img/sub-process-properties.jpg"><img src="img/sub-process-properties-300x212.jpg" alt="" title="sub-process-properties" width="300" height="212" class="alignnone size-medium wp-image-872" /></a><br />
Figure 50 &#8211; Sub process properties<br />
</div></p>
<p>
* First edit box: This is the sub composition name field. You can edit the name here,<br />
* Second edit box: This is a description field. You can write the sub composition description here,<br />
* Connector List: This is the list of all sub composition connectors. The name field is editable,<br />
* Apply button: Click on this button to apply your change,<br />
* Cancel button: Click on this button to discard your change.</p>
<p><a name="Error"></a><br />
<strong><br />



<h2>Error</h2>
<p><p style="text-align: right;"><a href="#">back to top</a></p>	
</strong></p>


<h3 style="text-align: left;">Highlighted</h3>
<p><div align="center">
<a href="img/Error0011.jpg"><img src="img/Error0011-300x156.jpg" alt="" title="Error001" width="300" height="156" class="alignnone size-medium wp-image-920" /></a><br />
Figure 51 &#8211; Highlighted<br />
</div></p>
<p>When an error occurs, Caméléon highlight in red erroneous components.</p>


<h3 style="text-align: left;">Browse error</h3>
<p><div align="center">
<a href="img/Error0021.jpg"><img src="img/Error0021-300x83.jpg" alt="" title="Error002" width="300" height="83" class="alignnone size-medium wp-image-921" /></a><br />
Figure 52 &#8211; Error list<br />
</div></p>
<p>You can browse error using the Execution problems tab on Caméléon Outputs (bottom part of the composer).<br />
Double click on an error id show on the composer or controller the component where the error occurs.</p>


<h3 style="text-align: left;">Lock pop-up</h3>
<p><div align="center">
Figure 53 &#8211; Lock pop-up<br />
</div></p>
<p>At stop or pause, a Lock pop-up may appears when a component has not yet finished its work. You can press the save &#038; restart button to relaunch Caméléon. It may be useful in debug mode when components has some infinite execution problems.</p>
<p><a name="CLI"></a><br />
<strong><br />



<h2>Command Line Interface (CLI)</h2>
<p><p style="text-align: right;"><a href="#">back to top</a></p>	
</strong><br />
In a command line (use git bash on windows), in the instance/bin directory, you can launch a composition like this:</p>
<pre>
./cameleon-cli.exe -mode=&quot;CLI&quot; -cm=&quot;../project/yourProject.cm&quot;
</pre>
<p>The help menu can be displayed with:</p>
<pre>
./cameleon-cli.exe -h

NAME
        Cameleon program launcher
SYNOPSIS
        ide.exe [options]
DESCRIPTION
        If no option is specified, it launch the .cm file specified in the Caméléon instance file
        -h:      print this help or print the .cm file help if -cm option is specified
        -cm=[.cm file path]:     launch a specific .cm file
        -mode=[IDE, GUI, CLI]:   set the Caméléon mode
                IDE:     launch Caméléon creator in IDE mode,
                GUI:     launch Caméléon app with the related GUI interface (only controller windows),
                CLI:     launch Caméléon in a command line mode (without any GUI),
EXAMPLES
        Standard launch with full IDE    cameleon.exe
        See the help of a specified .cm file     cameleon.exe -h -cm=&quot;../project/tutorial/example-0XY-Arguments/example-0XY-Arguments.cm&quot;
        Launch a specified .cm file with full IDE        cameleon.exe -cm=&quot;../project/tutorial/example-0XY-Arguments/example-0XY-Arguments.cm&quot;
        Launch a specified .cm file in command line application mode     cameleon.exe -mode=&quot;CMD&quot; -cm=&quot;../project/tutorial/example-0XY-Arguments/example-0XY-Arguments.cm&quot;
        Launch a specified .cm file in GUI application mode      cameleon.exe -mode=&quot;GUI&quot; -cm=&quot;../project/tutorial/example-0XY-Arguments/example-0XY-Arguments.cm&quot;
</pre>

<!-- You can start editing here. -->

</div>

