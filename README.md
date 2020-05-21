# a034_nc_chickadee
Ventilator Simulator Platform Synthesized From Published Mechanical Ventilator Design Literature Via FreeCAD Opensource CNC Machine Technology Using Arduino Boards/Shields

See: http://abbottanp.com/ for project engineering details.

While stuck in "lock-down" this project evolved and matured.  


Disclaimer:

1> The project presented here as a lowcost Ventilator Simulator Platform. It is not a qualified medical device at any level. This projects as presented here is not qualifed for any medical application. The project as presented should serve as a test platform and simulator supporting further development of the bio-medical equipment.

2> No warranty is made regarding the servicability or fitness for use of this low cost prototype ventilator. A user may employ, enjoy or engage in this project under the terms of the GNU General Public License (GNU GPL or GPL) as OpenSource software licenses that guarantee end users the freedom to run, study, share, and modify the software. All copyrights privildeges are reserved.


Evolution of Project:

The goal of this project was to gain a layman's level of understanding of the complexity of ventilators. This understanding was to be added to my CommonPlace Book of knowledge. While chasing this pursuit various seacrh strings associated with Forrest Bird led to the referensnces listed further below. The question eventually became one of "can CNC style machines possibly accomplish some level of ventilator fucntionality" using only components currently "on-hand"? Once that has been achieved can the resulting venitlator simulation platform serve a useful purpose as a learnig resource?
Working Assumptions:


Working Assumptions:

    +Ability to read technical material and translate into specification/work-product.
    +Experience creating Arduino project content within Sloeber or similar IDE.
    +Facility to adapt hardware as need to integrate with software to accomplish a task.
    +Implementing a design using only parts that are currently in the project's einventory.


Reference:

++ MIT Student Project: https://web.mit.edu/2.75/projects/DMD_2010_Al_Husseini.pdf

++ Theory of Operation: https://my.clevelandclinic.org/-/scassets/files/org/respiratory/fundamentals-of-mechanical-ventilation.ashx?la=en

++ Excellent Introduction Class: https://www.k-space.org/Class_Info/EE497/KAU_Ventilator_lec.pdf





Status:

1> Project completed to the point where additonal components would need to be purchased and incorporated.  The Arduino program module is functional at the Arduino Base Shield and Stack.  The program drives an X axis piston stepper and two angular motion  AB axes to control inspiration and expiration.  The most minimal breaths per minute for the Tidal Volume (expressed as mL) was demonstrated by the "model".  No air flow was achieved due to the lack of components. 

2> Using FreeCAD the mechanical aspects of the ventilator simulatior prototype platform was generated.  That lead to several evolutions in the approach ot the toolpaths needed for generting a gcode set of statements for integrating a "piston stroke cycle coupled with inspiration and expiration valve control. The proposed gcode soultion was verified at tthe Arduino stack level and under LinuxCNC.  

3> A routine process of converting a test subjects ventilation requirements into the volume and piston stroke length was achieved.  A basic web page input form collects the specification.  That is emailed to a receipent who makes a global repalcement (from "=" to ",") in the specification/data.  That data is then imported to a "canned" spreadsheet that parse the data elements into usable "gcode" content.  Loading that content to a file called "dilni.ngc" allows the "ventilator simulator" to executed cleanly.

4> It served as a great learning tool.



Closing:

Helpful comments are encouraged.  The spelling, grammar, and syntax will improve if the project moves forward from this point.
