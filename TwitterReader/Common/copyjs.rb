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
FilesToCopy = [
  "js/mosync.file.js",
  "js/mosync.mock.js",
  "js/app.js",
  "js/app.ui.js",
  "js/jqmobi.js",
  "js/wormhole.js"
]

# Target directories.
CopyTargets = [
  "../TwitterReaderIUI/LocalFiles/js/"
]

# Copy the files.
def copyFiles
  dirList = CopyTargets
  fileList = FilesToCopy
  dirList.each do |dirName|
    fileList.each do |fileName|
      FileUtils.cp(fileName, dirName)
    end
  end
end

# Call main function.
copyFiles
