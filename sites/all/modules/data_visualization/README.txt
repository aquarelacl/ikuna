DATA VISUALIZATION API

ABOUT
==========================
The data visualization api is a module that allows site builders to store
data visualizations as standardized JSON. The advantage to this is that the
user can select which rendering library to use to display that visualization,
based on which rendering libraries are currently installed.


VIZSTRUCT Notation
==========================
All data is stored in a standardized format called Vizstruct.  The spec for
this specified JSON can be found here: 
https://github.com/johnrobertwilson/visualization-spec/blob/master/spec.md .

The concept is that a visualization is represented by one or many series of
data.  A visualization can be as simple as a single series with no options,
such as a line with positive slope. A visualization can also be complex,
consisting of many series and accompanying options such as color, tooltips,
data labels and subtitles.


INSTALLATION
==========================
The Data Visualization API requires the Libraries, Entity API and Color
modules to be installed.

Navigate to 'admin/modules' and enable the module.

After that, you can add new data visualizations or import from CSV
at 'admin/content/data-visualization'.


LIBRARIES *IMPORTANT*
==========================
You must have at least one rendering library for the Data Visualization API
to render visualizations properly.

Place all library folders in either 'sites/all/libraries' or
'sites/<site_name>/libraries' depending on your setup. Each 
library folder name must be prefixed with 'jquery.'.  For example, if 
you install the Highcharts rendering library place it in 
'sites/all/libraries/jquery.highcharts'.

Highcharts: http://www.highcharts.com/download
JIT: http://thejit.org/
jqplot: https://bitbucket.org/cleonello/jqplot/downloads/

ADAPTERS *IMPORTANT*
===========================
Currently adapters for Highcharts, jqplot & JIT have been written and can be
found here.  https://github.com/treehouseagency/dataviz-adapters . You must
have the adapter installed for the library you want to render in.  The purpose
of the adapter is to translate the standardized json created by the data
visualization api and call that rendering library's unique functions structure.
Place all or the adapter file of your choice in the adapter folder of the
module.

CREATING VISUALIZATIONS
==========================
There are two ways to create data visualizations.

1) If you already have your data and options in Vizstruct format, navigate to
'data-visualization/add'.  Provide a title and a description, attach
the *.json file with the proper Vizstruct format, select the visualization
type, select which rendering library you want to use and then save.

2) If you do not have the data structured in Vizstruct, the module will
make the file for you if you have your data stored as CSV. Go to
'admin/content/data-visualization/import'. You can either paste in the CSV
or specify the location of the file on your hard drive.

Formatting the CSV:
One series per line. Each series must start with a string indicating its name.
ie SeriesName, 1, 2, 3, 4. Please delimit your values to commas
ie SeriesName, 1, 2, 3, 4. If you have x,y coordinates, contain each point 
with "[]", delimit them with "|" ie SeriesName,[1|2],[3|4],[5|6].
Strings can be intermixed with data ie SeriesName, [a|2], [b|3], [c|4] .


Placing Visualizations
==========================
For every Data Visualization that is created, a corresponding block is created.
