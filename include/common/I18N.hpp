#pragma once

#include <libintl.h>
#include <locale.h>

#define _(String) gettext(String)

#define N_(String) String