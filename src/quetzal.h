#ifndef QUETZAL_H
#define QUETZAL_H

#include <glib.h>

#define QUETZAL_NAME "Quetzal Text Editor"
#define QUETZAL_SHORTDESC "A simple and lightweight text and code editor." 

#define QUETZAL_MAJOR_VERSION (0)
#define QUETZAL_MINOR_VERSION (11) 
#define QUETZAL_MICRO_VERSION (1)

#define QUETZAL_VERSION (g_strdup_printf("%d.%d.%d", QUETZAL_MAJOR_VERSION, QUETZAL_MINOR_VERSION, QUETZAL_MICRO_VERSION))
#define QUETZAL_CHECK_VERSION (major, minor, micro) (\
  QUETZAL_MAJOR_VERSION > (major) || \
  QUETZAL_MAJOR_VERSION == (major) && QUETZAL_MINOR_VERSION > (minor) || \
  QUETZAL_MAJOR_VERSION == (major) && QUETZAL_MINOR_VERSION == (minor) && QUETZAL_MICRO_VERSION >= (micro) \
)

#endif /* QUETZAL_H */

