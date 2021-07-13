//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon*/                  /*Command*/           /*Interval*/    /*Signal*/
	{""               ,       "dwm-status -m",            3,             0,       },
	{""               ,       "dwm-status -a",            0,            10,       },
	{""               ,       "dwm-status -t",            8,             0,       },
	{""               ,       "dwm-status -b",            8,             0,       },
	{""               ,       "dwm-status -w",            2,             0,       },
	{"^c#83a598^󰃭 ^d^",       "date '+%b %d - %H:%M'",   60,             0,       },
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = " | ";
static unsigned int delimLen = 5;
