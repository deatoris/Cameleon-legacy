<div id="page-heading">
    <h1>Concepts (2.0.6)</h1>		
</div>
<!-- END page-heading -->

<div class="post full-width clearfix">
<p>Cameleon is a new free &#038; open source language based on an abstract functional grammar &#038; a glossary divided in specifics working fields dictionaries. The grammar is based on a petri net extension with a mathematical formalism described in <a href="http://arxiv.org/abs/1110.4802">Cameleon grammar, a petri net extension</a>. The language can be edited graphically using the Cameleon Algorithm Editor to create Cameleon program, named composition. The language glossary can be extended with new fields dictionaries using the Cameleon Dictionary Development Kit (DDK) to create Cameleon dictionary, a collection of words, them-self named operators. Cameleon language aims to simplify &#038; accelerate software development by using a high-level bottom-up approach.</p>
<p>With a simple drag &amp; drop mechanism, you can create an image processing prototype where the calibration is done in real-time. In Caméléon creator, code is a graphical data(work)-flow interpreted and executed by the Caméléon Virtual Machine with a mathematical equivalence &amp; a XML equivalence.</p>
<p>Before following tutorial or references to build your prototypes, you must know how to read the Caméléon language. To do so, you have to understand deeply Caméléon&#8217;s inner concepts. </p>
<p><a name="operator"></a><br />
<strong>An operator</strong> is an independent software component using input data x to produce output data y. An operator is like a <a href="http://en.wikipedia.org/wiki/Function_(mathematics)">mathematical function</a> f, f(x) = y.<br />
An operator has a graphical representation: a circle including its name and surrounded by little circles representing input (little circles including little black circles) and output (little circles including big black circles) datas.</p>
<p><center><br />
<br />
<a href="img/yfx1.jpg"><img src="img/yfx1-300x100.jpg" alt="" title="y=f(x)" width="300" height="100" class="alignnone size-medium wp-image-1514" /></a><br />
<em>Figure 1 &#8211; An operator and its equivalence</em><br />
</center></p>
<p><strong>Note:</strong> It exists a functional equivalence between operator, <a href="http://en.wikipedia.org/wiki/Function_(mathematics)">mathematical function</a> &#038; <a href="http://en.wikipedia.org/wiki/Subroutine">computer science function</a>. This is a good way to organize knowledge &#038; know-how.</p>
<p><a name="connector"></a><br />
Little circles surrounding the operator represents input &#038; output data and are named connectors. A connector hold a data and gives the ability to connect itself to another connector in order to share its data. All data has a type (NUMBER, STRING, IMAGE &#8230;).</p>
<p><center><br />
<br />
<a href="img/gy2.jpg"><img src="img/gy2-300x98.jpg" alt="" title="g(y)" width="300" height="98" class="alignnone size-medium wp-image-1519" /></a><br />
<em>Figure 2 &#8211; A first connection</em><br />
</center></p>
<p>An equivalence to Figure 2 is f(x)=y; g(y)=z.</p>
<p>Connection condition: You can only connect output connectors to input connectors with the same type or with a generic type. A connected connector cannot be connected to another connector.</p>
<p>A connection is represented by a line with an arrow showing the direction of the connection.</p>
<p><center><br />
<br />
<a href="img/connection.jpg"><img src="img/connection-300x137.jpg" alt="" title="connection" width="300" height="137" class="alignnone size-medium wp-image-1522" /></a><br />
<em>Figure 3 &#8211; A connection, a is shared to b</em><br />
</center></p>
<p><a name="control"></a><br />
<strong>A control</strong> is an independent software component using input data x to produce output data y with a graphical user interface enabling the user to view and/or manipulate data. y=g(x), the function g is a human action.</p>
<p><center><br />
<br />
<a href="img/contrl.jpg"><img src="img/contrl-300x98.jpg" alt="" title="contrl" width="300" height="98" class="alignnone size-medium wp-image-1524" /></a><br />
<em>Figure 4 &#8211; A control</em><br />
</center></p>
<p>Inputs and outputs data are representing by connectors as we do for the operator. Those connectors can be connected to operator&#8217;s connector.</p>
<p><center><br />
<br />
<em>Figure 5 &#8211; A control connection</em><br />
</center></p>
<p>It exists different kinds of controls.</p>
<p><center><br />
<br />
<a href="img/Controls.jpg"><img src="img/Controls-300x150.jpg" alt="" title="Controls" width="300" height="150" class="alignnone size-medium wp-image-759" /></a><br />
<em>Figure 6 &#8211; Controls</em><br />
</center></p>
<p><a name="dictionary"></a><br />
<strong>A dictionary</strong> is a separate Caméléon Editor plugin as a shared library offering several components (operator, control &#8230;) to enrich Caméléon Editor with new independent software component to play with.</p>
<p><strong>Note:</strong> You can add your own C++ component dictionary using the Caméléon Development Kit. Generally, dictionaries addresses specifics business fields. For instance, Population Dictionary target image processing &#038; analysis in material science.</p>
<p><a name="composition"></a><br />
<strong>A composition</strong> is a set of connected operators &#038; controls combined to process specifics numerical tasks. It&#8217;s a computer program with a graphical representation. A composition is also named a prototype.</p>
<p><center><br />
<br />
<em>Figure 7 &#8211; A composition</em><br />
</center></p>
<p><a name="sub"></a><br />
<strong>A sub composition</strong>, or a <strong>sub process</strong>, is a set of connected operators &#038; controls aggregate under a graphical grey element. It&#8217;s a graphical view to simplify &#038; organize a composition. You can browse a sub composition and modify its content.</p>
<p><center><br />
<br />
<em>Figure 8 &#8211; A sub composition, outer &#038; inner view</em><br />
</center></p>
<p><a name="pattern"></a><br />
<strong>A pattern</strong> is a reusable sub composition. The pattern creation is the same as the operator creation. A pattern is represented by a *.pa XML file in the pattern directory of the Caméléon instance directory. This file is sharable and can be used in all Caméléon instance.</p>
<p><center><br />
<br />
<em>Figure 9 &#8211; Pattern &#038; pattern directory</em><br />
</center></p>
<p><a name="interface"></a><br />
<strong>An interface</strong> is a composition composed exclusively by controls &#038; managed by the controller window.</p>
<p><center><br />
<br />
<em>Figure 10 &#8211; Interface</em><br />
</center></p>
<p>A composition can be launched &#038; stopped.</p>
<p><strong>The execution</strong> is directed by inputs &#038; outputs data. To know more about inner mechanics, please see <a href="http://arxiv.org/abs/1110.4802">the theory article</a>. </p>
<p>Connectors as a status regarding its inner data: NEW (GREEN), EMPTY (GREY) or OLD (BLUE).</p>
<p><center><br />
<br />
<em>Figure 11 &#8211; Connectors statuses</em><br />
</center></p>
<p>Operator as a status regarding its execution: RUNNABLE (GREEN), NOTRUNNABLE (GREY), RUNNING (YELLOW), ERROR (RED).<br />
An operator can only be runned if its <strong>execution condition</strong>, regarding connector statuses, is verified.</p>
<p><center><br />
<br />
<em>Figure 12 &#8211; Operator statuses</em><br />
</center></p>
<p>The standard execution condition is as follow:<br />
* inputs datas are NEW or OLD with a minimum of one input at NEW,<br />
* outputs datas are OLD or EMPTY.</p>
<p>Execution condition can be standard or specific to an operator. Please see the component specification &#038; documentation (<a href="http://www.shinoe.org/cameleon/scd-reference/">SCD</a>, <a href="http://www.shinoe.org/cameleon/population-dictionary-reference/">Population</a>).</p>
<p><center><br />
<br />
<em>Figure 13 &#8211; A composition execution</em><br />
</center></p>
<p>For instance, on the Figure 13, at execution time, if X is NEW, f is executed and produce Y. Then Y is dumped into Y&#8217;, then g is executed and produce Z.</p>
<p><center><br />
<br />
<em>Figure 14 &#8211; An other composition execution</em><br />
</center></p>
<p><strong>Note</strong>: At second execution of this part of the composition, the flow cardinality before &#038; after h is disturbed. Cb < Ca. We need to synchronize the two h inputs with a synchronous operator with a different kind of execution condition.</p>
<p><center><br />
<br />
<em>Figure 15 &#8211; Yet an other composition execution</em><br />
</center><br />
Merge doesn&#8217;t need a data available in the two inputs data to be RUNNABLE.<br />
Study the merge documentation and understand that it exists different kind of execution condition. You have to master them if you want to create robust prototypes. </p>
<p><strong>Update function</strong>.<br />
if example</p>
<p><strong>Some screen shots</strong></p>
<p><a href="/proto.jpg"><img src="img/proto-300x150.jpg" alt="" title="proto" width="300" height="150" class="alignnone size-medium wp-image-906" /></a><a href="img/proto2.jpg"><img src="img/proto2-300x150.jpg" alt="" title="proto2" width="300" height="150" class="alignnone size-medium wp-image-907" /></a><a href="img/prototyping1.jpg"><img src="img/prototyping1-300x151.jpg" alt="" title="prototyping" width="300" height="150" class="alignnone size-medium wp-image-908" /></a></p>
<p><a href="img/Navigation000.jpg"><img src="img/Navigation000-300x225.jpg" alt="" title="Navigation000" width="300" height="225" class="alignnone size-medium wp-image-854" /></a><a href="img/Navigation0011.jpg"><img src="img/Navigation0011-300x225.jpg" alt="" title="Navigation001" width="300" height="225" class="alignnone size-medium wp-image-883" /></a><a href="img/Navigation002.jpg"><img src="img/Navigation002-300x225.jpg" alt="" title="Navigation002" width="300" height="225" class="alignnone size-medium wp-image-856" /></a></p>
<p><a href="img/operatordata001.jpg"><img src="img/operatordata001-300x150.jpg" alt="" title="operatordata001" width="300" height="150" class="alignnone size-medium wp-image-901" /></a><a href="img/scd_full1.jpg"><img src="img/scd_full1-300x156.jpg" alt="" title="scd_full" width="300" height="150" class="alignnone size-medium wp-image-902" /></a><a href="img/operator4.jpg"><img src="img/operator4-300x150.jpg" alt="" title="operator" width="300" height="150" class="alignnone size-medium wp-image-903" /></a></p>
<p><a href="img/ctrl.jpg"><img src="img/ctrl-300x150.jpg" alt="" title="ctrl" width="300" height="150" class="alignnone size-medium wp-image-912" /></a><a href="img/Controls.jpg"><img src="img/Controls-300x150.jpg" alt="" title="Controls" width="300" height="150" class="alignnone size-medium wp-image-759" /></a><a href="img/proto3.jpg"><img src="img/proto3-300x150.jpg" alt="" title="proto3" width="300" height="150" class="alignnone size-medium wp-image-911" /></a></p>
<p><a href="img/Connection001.jpg"><img src="img/Connection001-300x235.jpg" alt="" title="Connection001" width="300" height="235" class="alignnone size-medium wp-image-833" /></a><a href="img/Connection002.jpg"><img src="img/Connection002-300x235.jpg" alt="" title="Connection002" width="300" height="235" class="alignnone size-medium wp-image-834" /></a><a href="img/Connection003.jpg"><img src="img/Connection003-300x235.jpg" alt="" title="Connection003" width="300" height="235" class="alignnone size-medium wp-image-835" /></a></p>
<p><a href="img/compo2.jpg"><img src="img/compo2-300x150.jpg" alt="" title="compo2" width="300" height="150" class="alignnone size-medium wp-image-914" /></a><a href="img/compo1.jpg"><img src="img/compo1-300x150.jpg" alt="" title="compo1" width="300" height="150" class="alignnone size-medium wp-image-915" /></a><a href="img/prototyping2.jpg"><img src="img/prototyping2-300x151.jpg" alt="" title="prototyping" width="300" height="150" class="alignnone size-medium wp-image-916" /></a></p>
<p><a href="img/sub.jpg"><img src="img/sub-300x150.jpg" alt="" title="sub" width="300" height="150" class="alignnone size-medium wp-image-760" /></a><a href="img/sub.jpg"><img src="img/sub-300x150.jpg" alt="" title="sub" width="300" height="150" class="alignnone size-medium wp-image-760" /></a><a href="img/sub.jpg"><img src="img/sub-300x150.jpg" alt="" title="sub" width="300" height="150" class="alignnone size-medium wp-image-760" /></a></p>
