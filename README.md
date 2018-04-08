# TMVA Tutorial at the 2nd IML Machine Learning Workshop 

These are tutorials on TMVA given at the 2nd  IML Machine Learning Workshop (9 April 2018)

See Workshop agenda at  https://indico.cern.ch/event/668017 


The tutorials consist of notebook which can be run on SWAN ( https://swan.cern.ch )


In order to run the tutorial we recommend that you:

1. Open a Terminal in SWAN 
2. Clone your repository (it will appear in your cernbox folder)
```
git clone --branch IML-tutorial-2018 https://github.com/lmoneta/tmva-tutorial.git
```
* The tutorials will be in the directory tutorial_IML2018

* The notebooks used in this tutorial use C++. A similar version using  Python interface of ROOT (PyROOT) and the Javascript based extension for notebook, `JSMVA` was done at the IML tutorial in 2017. they are available under the tutorial_IML2017 directory. (see also https://indico.cern.ch/event/595059 )

* More notebooks, some written also in Python, are available in the Machine Learning Gallery of SWAN, https://swan.web.cern.ch/content/machine-learning

* Addition tutorials of TMVA are available in the `tutorials/tmva` directory of the main ROOT git repository. These are available also here https://root.cern.ch/doc/master/group__tutorial__tmva.html

* The updated TMVA Users Guide is available on git, https://github.com/root-project/root/raw/master/documentation/tmva/UsersGuide/TMVAUsersGuide.pdf


## Swan and Jupyter notebooks quick start ##

If you never used jupyter notebooks, you can find some quickstart information at the following links [What is a jupyter notebook?](http://nbviewer.jupyter.org/github/jupyter/notebook/blob/master/docs/source/examples/Notebook/What%20is%20the%20Jupyter%20Notebook.ipynb), [Notebook Basics](http://nbviewer.jupyter.org/github/jupyter/notebook/blob/master/docs/source/examples/Notebook/Notebook%20Basics.ipynb),  [Running Code](http://nbviewer.jupyter.org/github/jupyter/notebook/blob/master/docs/source/examples/Notebook/Running%20Code.ipynb).

For information on swan, see the swan website: https://swan.web.cern.ch
In a nutshell, "SWAN (Service for Web based ANalysis) is a platform to perform interactive data analysis in the cloud". It gives you access to the LCG software stacks and to your CERNBOX files. 

* You need to have an active **CERNBOX** to use swan (https://cernbox.cern.ch/). If you don't have a CERN account, please contact us directly and some temporary accounts can be made available.
In that case you need to feel this form to get access to the account, https://docs.google.com/forms/d/1ed4LsFBg9BmAHvfWYJDL4Svdmfk-tB-xbIyIxMPUmuc/
We will provide you the account name and password. 

* If you think think the Jupyter notebook is stuck, open a terminal on swan and execute `top`: if the cpu is not being used (0%) by any of your processes, you may have to restart the Jupyter
kernel.

* For using the new Deep Learning features of TMVA (e.g. CNN) one needs now to use the *Development Bleeding Edge* Version of SWAN which is built using the current ROOT master version.
  There is a known issue with that version now, and you might need to add this line at the beginning of the notebook:  **gSystem->Load("libblas.so")**

**IMPORTANT:**    
* Please only have one notebook at the time on swan (you have to select the "running" tab and shutdown the notebook when done)
* Please avoid Chrome, it has known problems restarting kernels.
* Limit the number of events you use on swan (order 10000-100000) and the avoid expensive methods: *swan is meant for fast prototyping*. 
     * Each swan container has 2 GB of ram assigned, using the full dataset may hit the memory limit.

