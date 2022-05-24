# Impetus
I was screwing around with and learning OpenCV.  Their tutorials and the named window system is exceptionally easy and intuitive and as a result I quickly outgrew it.  As far as I know it.  For all I know I missed a whole namespace :stuck_out_tongue_winking_eye:

# Goals
I wanted something that ...
1. would make the main video view the biggest view
1. would have the intermediate step previews 
1. that those previews would be somehow arranged for me
1. had a somewhat approachable user interface
1. was somewhat aesthetically pleasing
1. was easy to mess around with and experiment

# Design overview

The main application layout consists of five video output windows.

In order to modify the processing chain edit function createProcessChain in src/ProcessChain.cpp.

# Boring notes for me
Because it can be months between coding sessions ...

## Signal handling
A signal handler is installed by main().  The signal handler sets a global flag.  MainWindow has a QTimer for periodically checking the global quit flag.  Once it flips to TRUE CentralWidget::close is called.  The slot does not return until the worker thread quits.  This is followed by QApplication::closeAllWindows.

## Logging
Log is to console via SPDLOG.  To configure manipulate the SPDLOG_LEVEL env variable.

```
export SPDLOG_LEVEL=info,mylogger=trace
```

https://github.com/gabime/spdlog