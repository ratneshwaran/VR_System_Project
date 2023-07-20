This project is on the development of VR system. 
Team members include team leader Ratneshwaran Maheswaran 
and the rest of the team: Phuttikarn Khanyoo, Naina Vaidya, Kevin Amanor, Hisham Salik.  

Description of Project 
This project aims to develop software for interactive simulation of CAD models, which enables users to modify features
such as colour, visibility and attaching and detaching parts.  The project is divided into two phases, with the first being 
the creation of a Graphical User Interface application that can load the model and allow for modifications. 
The second phase involves adding a VR output module to the software to develop compatibility with the VR system.


System Requirements/Softwares used
- Visual Studio 2019 or newer
- Cmake version 3.26
- QT creator 

Installation: 
- In order to run this project:
	- clone the repository onto a local desktop or download the zip folders
	- select the project folder for source and the build folder within the project for the build the binaries
	- click configure, generate and open project
	- The project should be selected as startup project and then built with local debugger 
	- This will then prompt the projects exe file to run
	- CAD files can be loaded onto the black screen which can then be prompted to run with Vr using Steam


Issues: 
OpenVr and VTK paths will have to manually be added when using Cmake to open project

Directories:
- The project has been split with exercise 1 representing retrieving a cylinder when the project is run 
- Ex2 combines the cylinder with our existing UI as made on QT
- Ex3 loads cad models which can be build upon using a treeview 
