#ifndef I18N_HPP
#define I18N_HPP

#include <libintl.h>
#include <locale.h>

#define _(String) gettext(String)

#define N_(String) String

#endif