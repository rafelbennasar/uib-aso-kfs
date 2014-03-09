kfs
===

ASO 2008-2009 Project.

We had to implement a file system based on inodes, providing
basic features like creating, reading, writing and deleting files organized in a
hierarchical tree directory structure.

A UNIX file system assigns one inode to each file in the file system; this
inode stores meta-data about the file and all pointers to its data blocks.
These data regions are divided in data blocks (and we can choose this size),
within which the file system stores file data and directory meta-data.
Directory entries contain file names and pointers to inodes.
