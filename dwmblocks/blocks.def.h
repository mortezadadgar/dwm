//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon*/                  /*Command*/           /*Interval*/    /*Signal*/
	{""               ,       "statusbar -m",            3,             0,       },
	{""               ,       "statusbar -a",            0,            10,       },
	{""               ,       "statusbar -t",            8,             0,       },
	{""               ,       "statusbar -b",            8,             0,       },
	{""               ,       "statusbar -w",            3,             0,       },
	{"^c#61AFEF^󰃭 ^d^",       "date '+%b %d - %H:%M'",   60,            0,       },
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = " | ";
static unsigned int delimLen = 5;
