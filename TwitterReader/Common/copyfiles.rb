#
# File: copyjs.rb
# Author: Mikael Kindborg
#
# This script copies files used by all versions of
# the TwitterReader app to the different project
# directories. Sharing JavaScript source code is
# curently not supported by MoSync, which is why
# the copy solution is taken to avoid code duplictaion.
#
require "fileutils"

# JavaScript files to be copied.
JSFilesToCopy = [
  "js/mosync.file.js",
  "js/mosync.mock.js",
  "js/app.js",
  "js/app.ui.js",
  "js/wormhole.js"
]

# C++ files to be copied. Not used!
#CPPFilesToCopy = [
#  "cpp/main.cpp",
#  "cpp/FileMessageHandler.h",
#  "cpp/FileMessageHandler.cpp"
#]

# Target directories.
CopyTargets = [
  "../TwitterReaderIUI/",
  "../TwitterReaderJQueryMobile/",
  "../TwitterReaderJQMobi/"
]

# Copy the files.
def copyFiles
  CopyTargets.each do |projectPath|
    copyFilesTo(projectPath + "LocalFiles/js/", JSFilesToCopy)
# We do not copy C++ files, they are included from the project.
#   copyFilesTo(projectPath, CPPFilesToCopy)
  end
end

def copyFilesTo(path, files)
  files.each do |fileName|
    if(!File.exist?(path))
      FileUtils.mkdir_p(path)
    end
    FileUtils.cp(fileName, path)
  end
end

# Call main function.
copyFiles
