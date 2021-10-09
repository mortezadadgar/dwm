/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

#define TERM "alacritty"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const Gap default_gap        = {.isgap = 1, .realgap = 10, .gappx = 10};
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 25;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "RobotoMono Nerd Font:size=10", "Material Design Icons:size=10" };
static const char dmenufont[]       = "RobotoMono Nerd Font:size=10";
static const char col_gray1[]       = "#282c34";
static const char col_gray2[]       = "#64656A"; /* gray */
static const char col_gray3[]       = "#dfdfdf"; /* white */
static const char col_gray4[]       = "#1d2026"; /* dark bkg */
static const char col_gray5[]       = "#8a8b91"; /* lighter gray */
static const char col_blue[]        = "#61AFEF";
static const char *colors[][4]      = {
	                   /*   fg         bg         border     float   */
	[SchemeNorm]     = { col_gray3, col_gray1,  col_gray2,  col_gray2 },
	[SchemeSel]      = { col_gray1, col_blue,   col_blue,   col_blue },
	[SchemeStatus]   = { col_gray3, col_gray4,  "#000000"  }, // Statusbar right
	[SchemeTagsSel]  = { col_blue,  col_gray4,  "#000000"  }, // Tagbar left selected
    [SchemeTagsNorm] = { col_gray3, col_gray4,  "#000000"  }, // Tagbar left unselected
    [SchemeInfoSel]  = { col_gray3, col_gray1,  "#000000"  }, // infobar middle  selected
    [SchemeInfoNorm] = { col_gray3, col_gray1,  "#000000"  }, // infobar middle  unselected
};
static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,8}, {2,6}, {0,8}, {0,0} }; /* represents the icon as an array of vertices */
static const XPoint stickyiconbb    = {4,8};	/* defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */

/* layout(s) */
	/*  class               instance  title            tags  mask  switchtotag    isfloating  monitor  scratch key */
	{   "firefox",          NULL,     NULL,  	        1   << 1,   1,             0,          -1,  0  },
	{   "TelegramDesktop",  NULL,     NULL,  	        1   << 2,   1,             0,          -1,  0  },
	{   "mpv",              NULL,     NULL,  	        1   << 3,   1,             0,          -1,  0  },
	{   "Zathura",          NULL,     NULL,  	        1   << 4,   1,             0,          -1,  0  },
	{   "Alacritty",        NULL,     "ncmpcpp",        1   << 5,   1,             1,          -1,  0  },
	{   "qBittorrent",      NULL,     NULL,  	        1   << 6,   1,             1,          -1,  0  },
	{   "libreoffice",      NULL,     NULL,  			1   << 7,   1,             0,          -1,  0  },
	{   NULL,               NULL,     "scratchpad",     0,          0,             1,          -1, 's' },
	{   NULL,               NULL,     "calc",           0,          0,             1,          -1, 'c' },
	{   NULL,               NULL,     "nnn",            0,          0,             1,          -1, 'n' },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */

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
static const char *dmenucmd[] = { "dmenu_run", "-bw", "2", "-c", "-l", "10", NULL };
static const char *termcmd[]  = { TERM, NULL };

/*First arg only serves to match against key in rules*/
static const char *scratchpadcmd[] = {"s", TERM, "-t", "scratchpad", "-o", "window.dimensions.columns=80", "-o", "window.dimensions.lines=26" ,NULL};
static const char *calc[] = {"c", TERM, "-t", "calc", "-e", "bc", "-q", NULL};
static const char *nnn[] = {"n", TERM, "-t", "nnn", "-e", "nnn", "-eu", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,						XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_u,      togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_c,      togglescratch,  {.v = calc } },
	{ Mod1Mask,                     XK_n,      togglescratch,  {.v = nnn } },
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
	{ MODKEY,						XK_s,	   togglefloating, {0} },
	{ MODKEY,                       XK_x,      togglesticky,   {0} },
	{ MODKEY,             			XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_minus,  setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = GAP_TOGGLE} },
	{ MODKEY|ShiftMask,             XK_f,      spawn,          SHCMD("firefox") },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          SHCMD("telegram-desktop") },
	{ MODKEY,                       XK_p,      spawn,          SHCMD("dmenupass") },
	{ MODKEY|ControlMask,           XK_p,      spawn,          SHCMD("dmenupower") },
	{ MODKEY,                       XK_Print,  spawn,          SHCMD("dmenumaim") },
	{ MODKEY,                       XK_n,      spawn,          SHCMD("dmenuiwd") },
	{ MODKEY,                       XK_e,      spawn,          SHCMD("dmenukill") },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD("open-ncmpcpp") },
	{ MODKEY|ControlMask,           XK_q,      spawn,          SHCMD("dwmout")} ,
	{ 0, XF86XK_MonBrightnessUp,    spawn,     SHCMD("xbacklight -inc 3") },
	{ 0, XF86XK_MonBrightnessDown,  spawn,     SHCMD("xbacklight -dec 3") },
	{ 0, XF86XK_AudioNext,          spawn,     SHCMD("mpc next; kill -45 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioPrev,          spawn,     SHCMD("mpc prev; kill -45 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioPlay,          spawn,     SHCMD("mpc toggle; kill -45 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioStop,          spawn,     SHCMD("mpc stop; kill -45 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,	   SHCMD("pamixer -i 5; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,	   SHCMD("pamixer -d 5; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioMute,		    spawn,	   SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
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

