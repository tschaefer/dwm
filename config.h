/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappx     = 10;       /* gaps between windows */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Terminus:pixelsize=14:antialias=false:autohint=false" };
static const char dmenufont[]       = "Terminus:pixelsize=14:antialias=false:autohint=false";
static const char col_bg_norm[]     = "#002b36";
static const char col_bd_norm[]     = "#586e75";
static const char col_fg_norm[]     = "#839496";
static const char col_fg_sel[]      = "#fdf6e3";
static const char col_bg_sel[]      = "#586e75";
static const char col_bd_sel[]      = "#839496";
static const char *colors[][3]      = {
	/*               fg           bg           border        */
	[SchemeNorm] = { col_fg_norm, col_bg_norm, col_bd_norm },
	[SchemeSel]  = { col_fg_sel,  col_bg_sel,  col_bd_sel  },
};

/* tagging */
static const char *tags[] = { "one", "two", "three", "four", "five", "six" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance    title       tags mask     isfloating   monitor */
	{ "Gimp",           NULL,       NULL,       0,            1,           -1 },
	{ "mpv",            NULL,       NULL,       0,            1,           -1 },
	{ "qemu-system",    NULL,       NULL,       0,            1,           -1 }
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
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
static const char *dmenucmd[]   = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_bg_norm, "-nf", col_fg_norm, "-sb", col_bg_sel, "-sf", col_fg_sel, NULL };

static const char *termcmd[]    = { "st", NULL };
static const char *slockcmd[]   = { "slock", NULL };
static const char *browsercmd[] = { "sensible-browser", NULL };

static const char *brighter[] = { "brightnessctl", "-q", "set", "5%+", NULL };
static const char *dimmer[]   = { "brightnessctl", "-q", "set", "5%-", NULL };

static const char *monitortoggle[] = { "monitor-dwm.sh", NULL };

static const char *upvol[]   = { "amixer", "-q", "sset", "Master", "2%+", "unmute", NULL };
static const char *downvol[] = { "amixer", "-q", "sset", "Master", "2%-", "unmute", NULL };
static const char *mutevol[] = { "amixer", "-q", "sset", "Master", "toggle", NULL };
static const char *mutemic[] = { "amixer", "-q", "sset", "Capture", "toggle", NULL };

static const char *playerprev[] = { "playerctl", "previous", NULL };
static const char *playerplay[] = { "playerctl", "play-pause", NULL };
static const char *playernext[] = { "playerctl", "next", NULL };

static Key keys[] = {
	/* modifier                     key                       function        argument */
	{ MODKEY,                       XK_p,                     spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,                spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_l,                     spawn,          {.v = slockcmd } },
	{ MODKEY|ShiftMask,             XK_b,                     spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_b,                     togglebar,      {0} },
	{ MODKEY,                       XK_j,                     focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                     focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,                     incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,                     incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                     setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                     setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return,                zoom,           {0} },
	{ MODKEY,                       XK_Tab,                   view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,                     killclient,     {0} },
	{ MODKEY,                       XK_t,                     setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                     setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                     setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,                 setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,                 togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,                     togglefullscr,  {0} },
	{ MODKEY,                       XK_0,                     view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                     tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                 focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                 tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                tagmon,         {.i = +1 } },
	{ 0,                            XF86XK_MonBrightnessUp,   spawn,          {.v = brighter } },
	{ 0,                            XF86XK_MonBrightnessDown, spawn,          {.v = dimmer } },
	{ 0,                            XF86XK_Display,           spawn,          {.v = monitortoggle } },
	{ 0,                            XF86XK_AudioRaiseVolume,  spawn,          {.v = upvol } },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn,          {.v = downvol } },
	{ 0,                            XF86XK_AudioMute,         spawn,          {.v = mutevol } },
	{ 0,                            XF86XK_AudioMicMute,      spawn,          {.v = mutemic } },
	{ 0,                            XF86XK_AudioPrev,         spawn,          {.v = playerprev } },
	{ 0,                            XF86XK_AudioPlay,         spawn,          {.v = playerplay } },
	{ 0,                            XF86XK_AudioNext,         spawn,          {.v = playernext } },
	TAGKEYS(                        XK_1,                                     0)
	TAGKEYS(                        XK_2,                                     1)
	TAGKEYS(                        XK_3,                                     2)
	TAGKEYS(                        XK_4,                                     3)
	TAGKEYS(                        XK_5,                                     4)
	TAGKEYS(                        XK_6,                                     5)
	{ MODKEY|ShiftMask,             XK_q,                     quit,           {0} },
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
