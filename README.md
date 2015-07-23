# Quetzal - Text Editor (beta)

Quetzal is an open source text and code editor written in C using the GTK+3 toolkit, taking advantage of the GTKSourceView, VTE and others tools to provide its features. It's the rebranding of an [old project](https://github.com/badwolfie/simple-text) of mine.

## Installation

Dependencies (package names may vary depending on your distribution):
* dconf
* glib-2.0 >= 2.44
* gtk+-3.0 >= 3.14
* gtksourceview-3.0 (>= 3.16 for background patterns)
* intltool >= 0.40
* libgee-0.8
* make (for project building)
* vte-2.91 >= 0.40
* autoconf (makedep)
* automake (makedep)
* git (makedep, not necessary if you downloaded the tarball)

Then, simply do:
```
	$ git clone https://github.com/badwolfie/quetzal.git
	$ cd quetzal
	$ ./autogen.sh --prefix=/usr
	$ make
	# make install
```

Or check the [releases page](https://github.com/badwolfie/quetzal/releases) for distribution-based installation options. 

## Shortcuts
| Key | Action |
|:---:|:---|
| Ctrl + B | Build project (You need to create a Makefile in the directory before using this feature) |
| Ctrl + Shift + C | Toggle embeded terminal |
| Ctrl + F | Toggle searching mode |
| Ctrl + N | Open a new file |
| Ctrl + O | Open a file |
| Ctrl + PageDown | Switch to next file |
| Ctrl + PageUp | Switch to previous file |
| Ctrl + Shift + P | Set syntax highlighting mode |
| Ctrl + Q | Quit application |
| Ctrl + R | Reload current file |
| Ctrl + S | Save current file |
| Ctrl + Shift + S | Save current file as... |
| Ctrl + Shift + T | Re-open last closed file |
| Ctrl + W | Close current file |
| Ctrl + Shift + W | Close all files |
| Ctrl + Z | Undo last step |
| Ctrl + Shift + Z | Redo last step |
| F1 | Open about window |
| Ctrl + F1 | Open preferences window |
| F10 | Open popover menu |
| F11 | Toggle fullscreen mode |

---

*Hope you really enjoy this project :)*
