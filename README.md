# TMVA Tutorial

These are tutorials on TMVA given at workshops and Machine Learning events in 2017 and 2018 

The most recent ones are :
* 3rd CMS Machine Learning workshop, June 2018, see agenda at https://indico.cern.ch/event/798721/
* the 1st Terascale School on Machine Learning at Desy, see agenda at https://indico.desy.de/indico/event/21278/


The tutorials consist mainly of Jupyter notebook which can be run on SWAN ( https://swan.cern.ch )

## Opening the Jupyter notebooks

The notebooks can be opened and run on SWAN ( swan.cern.ch ), the CERN Jupyter notebook service using your personal CERN account (for CERN users) or using temporary accounts which can be provided directly for the course/ 

The notebooks can also be used directly on your computer, if you have all the needed software installed, ROOT, Python, Jupyter and the machine learning software stack (Keras, scikit-learn and tensorflow).


### How to open the Jupiter notebooks in SWAN

Here we provide the step by step instructions on how to start working in SWAN. 


*   Click on the following link

[![SWAN](http://swanserver.web.cern.ch/swanserver/images/badge_swan_white_150.png)](https://cern.ch/swanserver/cgi-bin/go?projurl=https://github.com/lmoneta/ml-tutorial-EIROforum)

*  When starting SWAN a page appear to configure the environment. It is recommended to select the the default **95a** or **95a Python3** software stack.
For the other configuration parameters you can use the default values.

*  Click on  `Start my Session` at the button of the page. 

   *  Starting the session might take some time, especially if many users are doing it at the same time. 
   *  In case you observe an error when staring the session, it is likely that your CERNBox account has been not activated. In this case you should get a message: *Do you have CERNBox account? If not click [here](https://cernbox.cern.ch)*. 
   *  By clicking on the provided link, you will be to [CERNbox](https://cernbox.cern.ch) and, by doing this, your CERNBox account will be automatically validated.
   *  After having validated CERNBox you can click again on `Start my Session`. You might need to logout and login again if case an error occours.
   
*  When a SWAN session is started you will be in the `My Project` page. Click on the ``Download`` icon (*Download Project from git*) located at the top right next to the ``+`` icon. 

*  Enter the URL for the notebook GitHub repository (the link is also available in the Indigo agenda page)

```
https://github.com/lmoneta/tmva-tutorial.git
```


## How to download notebooks in your computer

Here we provide instructions on how to download the notebook directly from GitHub in your computer. 


1. Open a Terminal 
2. Clone your repository (it will appear in your cernbox folder)
```
git clone  https://github.com/lmoneta/tmva-tutorial.git
```

Note that you can also just download the notebook repository directly in SWAN from the SWAN terminal Window. By doing this the ``tmva-tutorial`` directory will be visible in the CERNBox page of SWAN. 


* The tutorials will be in the directory *notebooks*. Older tutorials could be retrieved using the corresponding git branches

## How to run notebooks in your computer

If you are using your own computer 
* Run the Jupyter notebook service by doing from the terminal
```jupyter notebook``  or ``root --notebook``
* A browser window will appear and the notebooks will be visible there and you can open a notebook by clicking on the link associated with its name. 



## SWAN and Jupyter notebooks quick start ##

Once you have access to the notebooks, after having open them, you can run interactively the code. The notebooks are capable of running code in a wide range of languages. However in this tutorial we will use the Python language. 

Here you can find a quick start information on what are the Jupyter notebooks and how to running code. 

* [What is a jupyter notebook?](http://nbviewer.jupyter.org/github/jupyter/notebook/blob/master/docs/source/examples/Notebook/What%20is%20the%20Jupyter%20Notebook.ipynb)
* [Notebook Basics](http://nbviewer.jupyter.org/github/jupyter/notebook/blob/master/docs/source/examples/Notebook/Notebook%20Basics.ipynb)
* [Running Code](http://nbviewer.jupyter.org/github/jupyter/notebook/blob/master/docs/source/examples/Notebook/Running%20Code.ipynb).

For information on using SWAN, see the SWAN website: https://swan.web.cern.ch

In a nutshell, **SWAN** (*Service for Web based Analysis*) is a platform to perform interactive data analysis in the cloud. It gives you access to a coherent software stacks for data analysis used at CERN (e.g. ROOT software framework, various scientific Python software packages, etc..) and to a storage facility (CERNBOX). 


* You need to have an active **CERNBox** to use SWAN. If you don't have a CERN account, please use the temporary accounts provided to you. As mentioned before, you might need to activate first the CERNBox account. This is done automatically the first time you go to https://cernbox.cern.ch

For getting the login credential of the temporary CERN accounts you need to sign first a form to agree on their usage and the [CERN Computing rules](https://security.web.cer.ch/security/rules/en/index.shtml) . 


## Some Important notes

* The notebooks used in this tutorial use either  C++ or Python. Python notebooks have the suffix *_py*  appended to their name. In some case a corresponding ROOT macro is available, which can  run
directly from the ROOT prompt. The macro are just an example how one can create easly a ROOT macro or a Python script from a given C++ or Python based notebook. 

* More notebooks, some written also in Python, are available in the Machine Learning Gallery of SWAN, https://swan.web.cern.ch/content/machine-learning

* Additional  tutorials of TMVA are available in the `tutorials/tmva` directory of the main ROOT git repository. You can find them also here https://root.cern.ch/doc/master/group__tutorial__tmva.html

* The updated TMVA Users Guide is available at https://root.cern.ch/download/doc/tmva/TMVAUsersGuide.pdf


* For using some of the latest new Deep Learning features of TMVA (e.g. CNN) one needs now to use the *Development Bleeding Edge* Version of SWAN which is built using the current ROOT master version.


* If you think think the Jupyter notebook is stuck, open a terminal on swan and execute `top`: if the cpu is not being used (0%) by any of your processes, you may have to restart the Jupyter
kernel.

* Please only have one notebook at the time on swan (you have to select the "running" tab and shutdown the notebook when done)
* Limit the number of events you use on swan (order 10000-100000) and the avoid expensive methods: *SWAN is meant for fast prototyping*. 
     * Each swan container has 2 GB of ram assigned, using a dataset with too many events may hit the memory limit.

