import subprocess
import argparse
import shutil
import os
import sys

from pathlib import Path
from os import listdir
from os.path import isfile, join

def copy_asset(source, dest):
    if not os.path.exists(dest):
        print("Create folder " + dest)
        os.makedirs(dest)
    print("source " + source)
    shutil.copy(source, dest)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--verbose', action='store_true', default=False, help='The full output will be shown')
    parser.add_argument('--textures', type=str, required=True, help='The folder containing the shaders')
    
    args = parser.parse_args()
    print("texture folder: " + str(args.textures))
    texture_files = [f for f in listdir(args.textures) if isfile(join(args.textures, f))]

    for texture in texture_files:
        texture_out = args.textures + texture
        if sys.platform == "linux":
            copy_asset(texture_out, "../bin/textures")
        elif sys.platform == "win32":
            out = Path("../bin/")
            if os.path.exists("../bin/Debug"):
                copy_asset(texture_out, "../bin/Debug/textures")
            else:
                copy_asset(texture_out, "../bin/Release/textures")


if __name__=="__main__":
    main()
