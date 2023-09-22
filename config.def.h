/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

#define TERM "st"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 10;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "JetBrainsMono Nerd Font:size=9:style=Bold", "Vazirmatn UI NL:size=9:style=Bold" };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/*  class               instance  title            tags  mask  switchtotag    isfloating  monitor scratch key */
	{   "firefox",          NULL,     NULL,  	        1   << 1,   1,             0,          -1,     0  },
	{   "chromium",         NULL,     NULL,  	        1   << 1,   1,             0,          -1,     0  },
	{   "TelegramDesktop",  NULL,     NULL,  	        1   << 2,   1,             0,          -1,     0  },
	{   "mpv",              NULL,     NULL,  	        1   << 3,   1,             0,          -1,     0  },
	{   "Zathura",          NULL,     NULL,  	        1   << 4,   1,             0,          -1,     0  },
	{   "qBittorrent",      NULL,     NULL,  	        1   << 7,   1,             1,          -1,     0  },
	{   NULL,               NULL,     "ncmpcpp",        1   << 5,   1,             1,          -1,     0  },
	{   NULL,               NULL,     "scratchpad",     0,          0,             1,          -1,    's' },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[D]",      deck },
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
static const char *termcmd[]  = { TERM, "-e", "tmux", NULL };

/*First arg only serves to match against key in rules*/
static const char *scratchpadcmd[] = {"s", TERM, "-t", "scratchpad", NULL};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_u,      togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_s,      togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
	{ MODKEY|ShiftMask,             XK_f,      spawn,          SHCMD("firefox") },
	{ MODKEY|ShiftMask,             XK_c,      spawn,          SHCMD("open-chromium") },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          SHCMD("telegram-desktop") },
	{ MODKEY,                       XK_p,      spawn,          SHCMD("dmenupass") },
	{ MODKEY|ControlMask,           XK_p,      spawn,          SHCMD("dmenupower") },
	{ MODKEY,                       XK_Print,  spawn,          SHCMD("maimpick") },
	{ MODKEY,                       XK_n,      spawn,          SHCMD("dmenuiwd") },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD("open-ncmpcpp") },
	{ 0, XF86XK_MonBrightnessUp,    spawn,     SHCMD("light -A 3") },
	{ 0, XF86XK_MonBrightnessDown,  spawn,     SHCMD("light -U 3") },
	{ 0, XF86XK_AudioNext,          spawn,     SHCMD("mpc -q next") },
	{ 0, XF86XK_AudioPrev,          spawn,     SHCMD("mpc -q prev") },
	{ 0, XF86XK_AudioPlay,          spawn,     SHCMD("mpc -q toggle") },
	{ 0, XF86XK_AudioStop,          spawn,     SHCMD("mpc -q stop") },
	{ 0, XF86XK_AudioRaiseVolume,   spawn,     SHCMD("amixer -q set Master 5%+; kill -RTMIN $(cat ~/.cache/pidofbar)") },
	{ 0, XF86XK_AudioLowerVolume,   spawn,     SHCMD("amixer -q set Master 5%-; kill -RTMIN $(cat ~/.cache/pidofbar)") },
	{ 0, XF86XK_AudioMute,          spawn,     SHCMD("amixer -q set Master toggle; kill -RTMIN $(cat ~/.cache/pidofbar)") },
	// non-multimedia keyboards
	{ 0,      XK_End,                    spawn,     SHCMD("mpc -q next") },
	{ 0,      XK_Home,                   spawn,     SHCMD("mpc -q prev") },
	{ MODKEY, XK_End,                    spawn,     SHCMD("mpc -q seek +5%") },
	{ MODKEY, XK_Home,                   spawn,     SHCMD("mpc -q seek -5%") },
	{ 0,      XK_Insert,                 spawn,     SHCMD("mpc -q toggle") },
	{ MODKEY, XK_Insert,                 spawn,     SHCMD("mpc -q stop") },
	{ 0,      XK_Page_Up,                spawn,     SHCMD("amixer -q set Master 5%+; kill -RTMIN $(cat ~/.cache/pidofbar)") },
	{ 0,      XK_Page_Down,              spawn,     SHCMD("amixer -q set Master 5%-; kill -RTMIN $(cat ~/.cache/pidofbar)") },
	{ 0,      XK_Pause,                  spawn,     SHCMD("amixer -q set Master toggle; kill -RTMIN $(cat ~/.cache/pidofbar)") },
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
static const Button buttons[] = {
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

