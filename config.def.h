/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

#define TERM "st"

/* appearance */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static unsigned int gappx     = 10;        /* gaps between windows */
static unsigned int snap      = 32;       /* snap pixel */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static int focusonwheel       = 0;
static int user_bh            = 25;       /* >= 1 means dwm will user_bh as bar height */
static char font[]            = "monospace:size=10";
static char dmenufont[]       = "monospace:size=10";
static const char *fonts[]          = { font };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm]   = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]    = { selfgcolor,  selbgcolor,  selbordercolor  },
       [SchemeTitle]  = { normfgcolor, normbgcolor, normbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/*  class               instance  title            tags  mask  switchtotag    isfloating  monitor */
	{   "Firefox",          NULL,     NULL,  	        1   << 1,   1,             0,          -1 },
	{   "TelegramDesktop",  NULL,     NULL,  	        1   << 2,   1,             0,          -1 },
	{   "mpv",              NULL,     NULL,  	        1   << 3,   1,             0,          -1 },
	{   "Zathura",          NULL,     NULL,  	        1   << 4,   1,             0,          -1 },
	{   "qBittorrent",      NULL,     NULL,  	        1   << 6,   1,             1,          -1 },
	{   NULL,               NULL,     "ncmpcpp",        1   << 5,   1,             1,          -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { TERM, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "font",               STRING,  &font },
		{ "dmenufont",          STRING,  &dmenufont },
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_s,      togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_f,      spawn,          SHCMD("firefox-bin || firefox") },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          SHCMD("telegram-desktop") },
	{ MODKEY,                       XK_p,      spawn,          SHCMD("dmenupass") },
	{ MODKEY|ControlMask,           XK_p,      spawn,          SHCMD("dmenupower") },
	{ MODKEY,                       XK_Print,  spawn,          SHCMD("dmenumaim") },
	{ MODKEY,                       XK_n,      spawn,          SHCMD("dmenuiwd") },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD("open-ncmpcpp") },
	{ 0, XF86XK_MonBrightnessUp,    spawn,     SHCMD("xbacklight -inc 3") },
	{ 0, XF86XK_MonBrightnessDown,  spawn,     SHCMD("xbacklight -dec 3") },
	{ 0, XF86XK_AudioNext,          spawn,     SHCMD("mpc -q next") },
	{ 0, XF86XK_AudioPrev,          spawn,     SHCMD("mpc -q prev") },
	{ 0, XF86XK_AudioPlay,          spawn,     SHCMD("mpc -q toggle") },
	{ 0, XF86XK_AudioStop,          spawn,     SHCMD("mpc -q stop") },
	{ 0, XF86XK_AudioRaiseVolume,   spawn,     SHCMD("amixer -q set Master 10%+; kill -34 $(cat ~/.cache/pidofbar)") },
	{ 0, XF86XK_AudioLowerVolume,   spawn,     SHCMD("amixer -q set Master 10%-; kill -34 $(cat ~/.cache/pidofbar)") },
	{ 0, XF86XK_AudioMute,          spawn,     SHCMD("amixer -q set Master toggle; kill -34 $(cat ~/.cache/pidofbar)") },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

