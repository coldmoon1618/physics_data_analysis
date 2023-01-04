#
# THIS IS A TEMPORARY WORKAROUND for the October 2018 tutorial
# uncomment it if needed
#
import sys, os
sys.path.insert(0, os.path.expanduser("~ritter/fixed_ROOT_import/"))

# Wide style
from ipython_tools import handler
handler.style()

# load the needed python libraries
import glob
import sys
import shutil
import subprocess

#
# CONFIGURATION PART
#

# Where do you want to store the data? (relative to working directory)
# >>>>>>>>> VERY IMPORTANT: THE CONTENT OF THIS FOLDER WILL BE DELETED EVERY TIME YOU RUN BELOW SCRIPT! <<<<<<
#outFolder = 'myOutputFolder'
outFolder = 'proc11_exp8_r43_200'

# Name prefix of output file
outFilePrefix = "Data_phi"

# mdst, cdst or dst?
dataType = 'mdst'

# How many jobs you want to submit PER RUN, at most?
nJobsPerType = 99

# What is the name of the steering file you want to submit?
steeringFile = 'Data_phi_selection.py'

# Run range over which you want to loop
firstRun = 43
lastRun = 200

# Do you want to run interactively?
runInteractively = False

# How many events per job do you want to process?
# 0 or negative to run on all the events
nEvts = 0

# Location of the data you want to repocess, i.e. lcation of the folder with all the r00*/ folders
rootFolder = '/group/belle2/dataprod/Data/release-04-02-02/DB00000938/proc11/e0008/4S/GoodRuns'
#rootFolder = '/group/belle2/dataprod/Data/release-04-02-02/DB00000938/proc11/e0010/4S/GoodRuns'

# Creates empty folders to store the output
if os.path.exists(outFolder):
    shutil.rmtree(outFolder)
os.makedirs(os.path.join(outFolder, 'logs'))
os.makedirs(os.path.join(outFolder, 'outs'))

# Convert paths to absolute paths.
# This is important if the user specifies an absolute path to their steeringFile.
outFolder = os.path.abspath(outFolder)
steeringFile = os.path.abspath(steeringFile)
rootFolder = os.path.abspath(rootFolder)

# now loops over the run numbers, and if a data are found for that run, processes them
for runnumber in range(firstRun, lastRun):   
    inputFolder = os.path.join(rootFolder, 'r{:0=5d}'.format(runnumber), 'skim/hlt_hadron', dataType, 'sub00')
    
    # Skip if folder doesn't exist.
    if not os.path.isdir(inputFolder):
        print("Folder {} does not exist. Skipping.".format(inputFolder))
        continue
        
    # take all files from the directory and combine them to an argument string for basf2
    files = glob.glob(os.path.join(inputFolder, '*.root'))
    
    nFilesPerJob = int(len(files) / nJobsPerType) + 1
    
    print('*****************************************************')
    print('Found {} file(s) in folder {}. Submitting {} file(s) per job.'.format(
              len(files),
              inputFolder, 
              nFilesPerJob)
         )
    print('*****************************************************')

    
    for iJob in range(0, nJobsPerType):
        
        # Set up log file names
        logFile = os.path.join(outFolder, 'logs', 'log_run{}_job{}.log'.format(runnumber, iJob))
        errFile = os.path.join(outFolder, 'logs', 'log_run{}_job{}.err'.format(runnumber, iJob))
        
        # Set up output options
        outFilePrefix = os.path.join(outFolder, 'outs', outFilePrefix)
        outFileSuffix = "run{}_job{}".format(runnumber, iJob)
        
        # Files to be submitted with _this_ job
        jobFiles = files[ iJob * nFilesPerJob : (iJob+1) * nFilesPerJob ]
        if not jobFiles:
            break

        # Build up command to run/submit job
        
        # Build up command to run basf2
        command = ['basf2']
        if nEvts > 0 :
            command += ["-n", str(nEvts)]
        for f in jobFiles:
            command += ["-i", f]
        command += [steeringFile, outFileSuffix, outFilePrefix] 
        
        # If we submit to queue: Prepend command for queue submission
        if not runInteractively:
            command = ['bsub',  '-q', 's',  '-oo', logFile, '-e', errFile] + command
            
        # Submit/run job 
        print(" ".join(command))
        subprocess.call(command)
        print()

