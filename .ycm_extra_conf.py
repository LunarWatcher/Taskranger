"""

Welcome to my standard .ycm_extra_conf.py. This aims to tackle Conan with newer versions of YCM, as well
as managing my weird-ass combined project structure of mixed dependencies. 

There are some default flags defined in this file; these are widely used in my projects, and will therefore be 
true in most cases. These can be modified if, for some reason, there's a mismatch.

This shouldn't affect Vim on Windows, provided clangd is used. Hopefully :fingers_crossed:
"""

import os
import json
import ycm_core
import logging


_logger = logging.getLogger(__name__)

# Base flags for all projects
flags = [
    "-std=c++17",
    "-pedantic",
    "-Wall",
    "-Wextra",
    "-Wno-c++11-narrowing",
    "-Isrc/",
]

def DirectoryOfThisScript():
  return os.path.dirname(os.path.abspath( __file__ ))
print(DirectoryOfThisScript())
# Config load 

# Load static flags 
if os.path.isfile(DirectoryOfThisScript() + "/staticinclude.txt"):
    with open(DirectoryOfThisScript() + "/staticinclude.txt", "r") as f:
        lines = f.readlines()
        for line in lines:
            flags.append(line.replace("\n", ""))


# Load conan flags
if os.path.isfile(DirectoryOfThisScript() + "/conan_ycm_flags.json"):
    conan_flags = json.loads(open(DirectoryOfThisScript() + "/conan_ycm_flags.json", "r").read())

    flags.extend(conan_flags["flags"])
    flags.extend(conan_flags["defines"])
    flags.extend(conan_flags["includes"])


SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c', '.m', '.mm' ]

def GetAbsolutePath(include_path, working_directory):
    if os.path.isabs(include_path):
        return include_path
    return os.path.join(working_directory, include_path)


def MakeRelativePathsInFlagsAbsolute( flags, working_directory ):
  if not working_directory:
    return list( flags )
  new_flags = []
  make_next_absolute = False
  path_flags = [ '-isystem', '-I', '-iquote', '--sysroot=' ]
  for flag in flags:
    new_flag = flag

    if make_next_absolute:
      make_next_absolute = False
      new_flag = GetAbsolutePath(flag, working_directory)

    for path_flag in path_flags:
      if flag == path_flag:
        make_next_absolute = True
        break

      if flag.startswith( path_flag ):
        path = flag[ len( path_flag ): ]
        new_flag = flag[:len(path_flag)] + GetAbsolutePath(path, working_directory)
        break

    if new_flag:
      new_flags.append( new_flag )
  return new_flags

compilation_database_folder = ''

if os.path.exists( compilation_database_folder ):
  database = ycm_core.CompilationDatabase( compilation_database_folder )
else:
  database = None


def IsHeaderFile( filename ):
  extension = os.path.splitext( filename )[ 1 ]
  return extension in [ '.h', '.hxx', '.hpp', '.hh' ]


def FindCorrespondingSourceFile( filename ):
  if IsHeaderFile( filename ):
    basename = os.path.splitext( filename )[ 0 ]
    for extension in SOURCE_EXTENSIONS:
      replacement_file = basename + extension
      if os.path.exists( replacement_file ):
        return replacement_file
  return filename

def Settings(**kwargs):
  relative_to = None
  compiler_flags = None
  filename = FindCorrespondingSourceFile(kwargs['filename'])
  if database:
    # Bear in mind that compilation_info.compiler_flags_ does NOT return a
    # python list, but a "list-like" StringVec object
    compilation_info = database.GetCompilationInfoForFile(filename)
    if compilation_info:
      compiler_flags = compilation_info.compiler_flags_
    else: compiler_flags = None
  else:
    relative_to = DirectoryOfThisScript()
    compiler_flags = flags

  final_flags = MakeRelativePathsInFlagsAbsolute( compiler_flags, relative_to )
  for flag in final_flags:
      if flag.startswith("-W"):
          final_flags.remove(flag)
  _logger.info("Final flags for %s are %s" % (filename, ' '.join(final_flags)))

  return {
    'flags': final_flags + ["-I/usr/include", "-I/usr/include/c++/9"],
    'do_cache': True
  }
