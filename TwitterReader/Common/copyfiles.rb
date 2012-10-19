#
# File: copyjs.rb
# Author: Mikael Kindborg
#
# Copy files used by the TwitterReader app
# to the different projects. In MoSync C/C++
# files can be included from a different
# directory, but wil JavaScript files, they
# must go in the LocalFiles folder of the
# project, therefore we copy these files
# using this script, to avoid code duplication.
#
require "fileutils"

# JavaScript files to be copied.
JSFilesToCopy = [
  "js/mosync.file.js",
  "js/mosync.mock.js",
  "js/app.js",
  "js/app.ui.js",
  "js/jqmobi.js",
  "js/wormhole.js"
]

# C++ files to be copied.
CPPFilesToCopy = [
  "cpp/main.cpp",
  "cpp/FileMessageHandler.h",
  "cpp/FileMessageHandler.cpp"
]

# Target directories.
CopyTargets = [
  "../TwitterReaderIUI/",
  "../TwitterReaderJQueryMobile/"
]

# Copy the files.
def copyFiles
  CopyTargets.each do |projectPath|
    copyFilesTo(
	  projectPath + "LocalFiles/js/",
      JSFilesToCopy)
    copyFilesTo(
	  projectPath,
      CPPFilesToCopy)
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