#define IDI_ICON			10
#define IDI_TRAY			11
#define IDI_TRAY_RECORDING	12
#define IDI_PROGRAMGUIDE	15

#define IDC_GRAB1	18
#define IDC_GRAB2	19

#define IDM_MENU					20
#define IDM_RECORD					21
#define IDM_CAPTURE					22
#define IDM_BUFFERING				23
#define IDM_ERROR					24
#define IDM_TIME					25
#define IDM_PROGRAMGUIDE			27
#define IDM_CHANNELSCAN				28
#define IDM_TRAY					29
#define IDM_CAPTUREPREVIEW			30
#define IDM_PLUGIN					31
#define IDM_CHANNELPANEL			32
#define IDM_INFORMATIONPANEL		33
#define IDM_SIDEBAR					34
#define IDM_COLORSCHEME				35

#define IDA_ACCEL	39

#define IDB_LOGO				40
#define IDB_LOGO32				41
#define IDB_CAPTURE				42
#define IDB_TITLEBAR			43
#define IDB_HDUSCONTROLLER		44
#define IDB_HDUSCONTROLLER_SEL	45
#define IDB_PANSCAN				46
#define IDB_SIDEBAR				47
#define IDB_THUMBBAR			48

#define IDD_ABOUT					50
#define IDD_INITIALSETTINGS			51
#define IDD_OPTIONS					52
#define IDD_OPTIONS_GENERAL			53
#define IDD_OPTIONS_VIEW			54
#define IDD_OPTIONS_OSD				55
#define IDD_OPTIONS_STATUS			56
#define IDD_OPTIONS_SIDEBAR			57
#define IDD_OPTIONS_PANEL			58
#define IDD_OPTIONS_COLORSCHEME		59
#define IDD_OPTIONS_OPERATION		60
#define IDD_OPTIONS_ACCELERATOR		61
#define IDD_OPTIONS_HDUSCONTROLLER	62
#define IDD_OPTIONS_DRIVER			63
#define IDD_OPTIONS_AUDIO			64
#define IDD_OPTIONS_RECORD			65
#define IDD_OPTIONS_CAPTURE			66
#define IDD_OPTIONS_CHANNELSCAN		67
#define IDD_OPTIONS_EPG				68
#define IDD_OPTIONS_PROGRAMGUIDE	69
#define IDD_OPTIONS_PLUGIN			70
#define IDD_OPTIONS_NETWORKREMOCON	71
#define IDD_OPTIONS_LOG				72
#define IDD_RECORDOPTION			80
//#define IDD_RECORDSTOPTIME			81
#define IDD_CHANNELSCAN				85
#define IDD_CHANNELPROPERTIES		86
#define IDD_PROGRAMGUIDETOOL		87
#define IDD_ERROR					90
#define IDD_STREAMINFO				91

#define CM_ZOOM_FIRST				100
#define CM_ZOOM_LAST				108
#define CM_ZOOM_20					(CM_ZOOM_FIRST+0)
#define CM_ZOOM_25					(CM_ZOOM_FIRST+1)
#define CM_ZOOM_33					(CM_ZOOM_FIRST+2)
#define CM_ZOOM_50					(CM_ZOOM_FIRST+3)
#define CM_ZOOM_66					(CM_ZOOM_FIRST+4)
#define CM_ZOOM_75					(CM_ZOOM_FIRST+5)
#define CM_ZOOM_100					(CM_ZOOM_FIRST+6)
#define CM_ZOOM_150					(CM_ZOOM_FIRST+7)
#define CM_ZOOM_200					(CM_ZOOM_FIRST+8)
#define CM_ASPECTRATIO				110
#define CM_ASPECTRATIO_FIRST		111
#define CM_ASPECTRATIO_LAST			116
#define CM_ASPECTRATIO_DEFAULT		111
#define CM_ASPECTRATIO_16x9			112
#define CM_ASPECTRATIO_LETTERBOX	113
#define CM_ASPECTRATIO_SUPERFRAME	114
#define CM_ASPECTRATIO_SIDECUT		115
#define CM_ASPECTRATIO_4x3			116
#define CM_FULLSCREEN				118
#define CM_ALWAYSONTOP				119
#define CM_VOLUME_UP				120
#define CM_VOLUME_DOWN				121
#define CM_VOLUME_MUTE				122
#define CM_STEREO_THROUGH			125
#define CM_STEREO_LEFT				126
#define CM_STEREO_RIGHT				127
#define CM_SWITCHAUDIO				128
#define CM_RECORD					130
#define CM_RECORD_START				131
#define CM_RECORD_STOP				132
#define CM_RECORD_PAUSE				133
#define CM_RECORDOPTION				134
//#define CM_RECORDSTOPTIME			135
#define CM_RECORDEVENT				136
#define CM_OPTIONS_RECORD			137
#define CM_EXITONRECORDINGSTOP		138
#define CM_DISABLEVIEWER			140
//#define CM_AUDIOONLY				141
#define CM_COPY						150
#define CM_SAVEIMAGE				151
#define CM_CAPTURE					152
#define CM_CAPTUREPREVIEW			153
#define CM_CAPTUREOPTIONS			154
#define CM_OPENCAPTUREFOLDER		155
#define CM_CAPTURESIZE_ORIGINAL		160
#define CM_CAPTURESIZE_VIEW			161
#define CM_CAPTURESIZE_75			162
#define CM_CAPTURESIZE_66			163
#define CM_CAPTURESIZE_50			164
#define CM_CAPTURESIZE_33			165
#define CM_CAPTURESIZE_1920X1080	166
#define CM_CAPTURESIZE_1440X810		167
#define CM_CAPTURESIZE_1280X720		168
#define CM_CAPTURESIZE_1024X576		169
#define CM_CAPTURESIZE_960x540		170
#define CM_CAPTURESIZE_800X450		171
#define CM_CAPTURESIZE_640X360		172
#define CM_CAPTURESIZE_1440X1080	173
#define CM_CAPTURESIZE_1280X960		174
#define CM_CAPTURESIZE_1024X768		175
#define CM_CAPTURESIZE_800X600		176
#define CM_CAPTURESIZE_720X540		177
#define CM_CAPTURESIZE_640X480		178
#define CM_CAPTURESIZE_FIRST		CM_CAPTURESIZE_ORIGINAL
#define CM_CAPTURESIZE_LAST			CM_CAPTURESIZE_640X480
#define CM_RESET					180
#define CM_RESETVIEWER				181
#define CM_INFORMATION				182
#define CM_PROGRAMGUIDE				183
#define CM_TITLEBAR					184
#define CM_STATUSBAR				185
#define CM_SIDEBAR					186
#define CM_CUSTOMTITLEBAR			187
#define CM_DECODERPROPERTY			188
#define CM_OPTIONS					189
#define CM_CLOSE					190
#define CM_EXIT						191
#define CM_SHOW						192
#define CM_STREAMINFO				193
#define CM_CHANNEL_UP				200
#define CM_CHANNEL_DOWN				201
#define CM_MENU						210
#define CM_ACTIVATE					211
#define CM_MINIMIZE					212
#define CM_MAXIMIZE					213
#define CM_ENABLEBUFFERING			220
#define CM_RESETBUFFER				221
#define CM_RESETERRORCOUNT			222
#define CM_SHOWRECORDREMAINTIME		225
#define CM_SHOWTOTTIME				230
#define CM_ADJUSTTOTTIME			231
#define CM_CHANNELPANEL_UPDATE		240
#define CM_CAPTURESTATUSBAR			250
#define CM_CHANNELMENU				260
#define CM_SERVICEMENU				261
#define CM_TUNINGSPACEMENU			262
#define CM_RECENTCHANNELMENU		263

#define CM_SIDEBAR_PLACE_LEFT		270
#define CM_SIDEBAR_PLACE_RIGHT		271
#define CM_SIDEBAR_PLACE_TOP		272
#define CM_SIDEBAR_PLACE_BOTTOM		273
#define CM_SIDEBAR_PLACE_FIRST		CM_SIDEBAR_PLACE_LEFT
#define CM_SIDEBAR_PLACE_LAST		CM_SIDEBAR_PLACE_BOTTOM
#define CM_SIDEBAROPTIONS			274

#define CM_CHANNEL_FIRST	300
#define CM_CHANNEL_LAST		599
#define CM_SPACE_ALL		600
#define CM_SPACE_FIRST		601
#define CM_SPACE_LAST		649
#define CM_SERVICE_FIRST	650
#define CM_SERVICE_LAST		699
#define CM_DRIVER_FIRST		700
#define CM_DRIVER_LAST		798
#define CM_DRIVER_BROWSE	799

#define CM_VOLUMENORMALIZE_FIRST	800
#define CM_VOLUMENORMALIZE_LAST		(CM_VOLUMENORMALIZE_FIRST+3)
#define CM_VOLUMENORMALIZE_NONE		(CM_VOLUMENORMALIZE_FIRST+0)
#define CM_VOLUMENORMALIZE_125		(CM_VOLUMENORMALIZE_FIRST+1)
#define CM_VOLUMENORMALIZE_150		(CM_VOLUMENORMALIZE_FIRST+2)
#define CM_VOLUMENORMALIZE_200		(CM_VOLUMENORMALIZE_FIRST+3)

#define CM_AUDIOSTREAM_FIRST		810
#define CM_AUDIOSTREAM_LAST			819

#define CM_INFORMATIONPANEL_ITEM_FIRST			820
#define CM_INFORMATIONPANEL_ITEM_VIDEO			(CM_INFORMATIONPANEL_ITEM_FIRST+0)
#define CM_INFORMATIONPANEL_ITEM_VIDEODECODER	(CM_INFORMATIONPANEL_ITEM_FIRST+1)
#define CM_INFORMATIONPANEL_ITEM_VIDEORENDERER	(CM_INFORMATIONPANEL_ITEM_FIRST+2)
#define CM_INFORMATIONPANEL_ITEM_AUDIODEVICE	(CM_INFORMATIONPANEL_ITEM_FIRST+3)
#define CM_INFORMATIONPANEL_ITEM_SIGNALLEVEL	(CM_INFORMATIONPANEL_ITEM_FIRST+4)
#define CM_INFORMATIONPANEL_ITEM_ERROR			(CM_INFORMATIONPANEL_ITEM_FIRST+5)
#define CM_INFORMATIONPANEL_ITEM_RECORD			(CM_INFORMATIONPANEL_ITEM_FIRST+6)
#define CM_INFORMATIONPANEL_ITEM_PROGRAMINFO	(CM_INFORMATIONPANEL_ITEM_FIRST+7)

#define CM_PROGRAMGUIDE_UPDATE				900
#define CM_PROGRAMGUIDE_ENDUPDATE			901
#define CM_PROGRAMGUIDE_REFRESH				902
#define CM_PROGRAMGUIDE_TODAY				903
#define CM_PROGRAMGUIDE_TOMORROW			904
#define CM_PROGRAMGUIDE_DAYAFTERTOMORROW	905
#define CM_PROGRAMGUIDE_2DAYSAFTERTOMORROW	906
#define CM_PROGRAMGUIDE_3DAYSAFTERTOMORROW	907
#define CM_PROGRAMGUIDE_4DAYSAFTERTOMORROW	908
#define CM_PROGRAMGUIDE_5DAYSAFTERTOMORROW	909
#define CM_PROGRAMGUIDE_IEPGASSOCIATE		910
#define CM_PROGRAMGUIDE_DRAGSCROLL			911
#define CM_PROGRAMGUIDE_TUNINGSPACE_ALL		930
#define CM_PROGRAMGUIDE_TUNINGSPACE_FIRST	931
#define CM_PROGRAMGUIDE_TUNINGSPACE_LAST	949
#define CM_PROGRAMGUIDE_DRIVER_FIRST		950
#define CM_PROGRAMGUIDE_DRIVER_LAST			999

#define CM_PLUGIN_FIRST		2000
#define CM_PLUGIN_LAST		2999

#define CM_SPACE_CHANNEL_FIRST	3000
#define CM_SPACE_CHANNEL_LAST	6999

#define CM_PROGRAMGUIDETOOL_FIRST	7000
#define CM_PROGRAMGUIDETOOL_LAST	7999

#define CM_CHANNELHISTORY_FIRST		8000
#define CM_CHANNELHISTORY_LAST		8098
#define CM_CHANNELHISTORY_CLEAR		8099

#define CM_PLUGINCOMMAND_FIRST		9000
#define CM_PLUGINCOMMAND_LAST		9999

#define CM_CHANNELNO_FIRST	10000
#define CM_CHANNELNO_LAST	(CM_CHANNELNO_FIRST+999)
#define CM_CHANNELNO_1		(CM_CHANNELNO_FIRST+0)
#define CM_CHANNELNO_2		(CM_CHANNELNO_FIRST+1)
#define CM_CHANNELNO_3		(CM_CHANNELNO_FIRST+2)
#define CM_CHANNELNO_4		(CM_CHANNELNO_FIRST+3)
#define CM_CHANNELNO_5		(CM_CHANNELNO_FIRST+4)
#define CM_CHANNELNO_6		(CM_CHANNELNO_FIRST+5)
#define CM_CHANNELNO_7		(CM_CHANNELNO_FIRST+6)
#define CM_CHANNELNO_8		(CM_CHANNELNO_FIRST+7)
#define CM_CHANNELNO_9		(CM_CHANNELNO_FIRST+8)
#define CM_CHANNELNO_10		(CM_CHANNELNO_FIRST+9)
#define CM_CHANNELNO_11		(CM_CHANNELNO_FIRST+10)
#define CM_CHANNELNO_12		(CM_CHANNELNO_FIRST+11)

#define SC_ABOUT			0x1000
#define SC_DOCKING			0x1001

#define IDC_VIEW			1000
#define IDC_VIDEOCONTAINER	1001
#define IDC_STATUS			1002
#define IDC_TITLEBAR		1003
#define IDC_SIDEBAR			1004

#define IDC_ABOUT_LOGO		1000

#define IDC_ERROR_ICON			1000
#define IDC_ERROR_MESSAGE		1001
#define IDC_ERROR_COPY			1002

#define IDC_INITIALSETTINGS_LOGO				1000
#define IDC_INITIALSETTINGS_DRIVER				1001
#define IDC_INITIALSETTINGS_DRIVER_BROWSE		1002
#define IDC_INITIALSETTINGS_MPEG2DECODER		1003
#define IDC_INITIALSETTINGS_VIDEORENDERER		1004
#define IDC_INITIALSETTINGS_CARDREADER			1005
#define IDC_INITIALSETTINGS_SEARCHCARDREADER	1006
#define IDC_INITIALSETTINGS_RECORDFOLDER		1007
#define IDC_INITIALSETTINGS_RECORDFOLDER_BROWSE	1008
#define IDC_INITIALSETTINGS_HELP				1010

#define IDC_OPTIONS_LIST		1000
#define IDC_OPTIONS_TITLE		1001
#define IDC_OPTIONS_PAGEPLACE	1002
#define IDC_OPTIONS_HELP		1003

#define IDC_OPTIONS_DEFAULTDRIVER_NONE			1000
#define IDC_OPTIONS_DEFAULTDRIVER_LAST			1001
#define IDC_OPTIONS_DEFAULTDRIVER_CUSTOM		1002
#define IDC_OPTIONS_DEFAULTDRIVER				1003
#define IDC_OPTIONS_DEFAULTDRIVER_BROWSE		1004
#define IDC_OPTIONS_DECODER						1005
#define IDC_OPTIONS_RENDERER					1006
#define IDC_OPTIONS_CARDREADER					1007
#define IDC_OPTIONS_RESIDENT					1008
#define IDC_OPTIONS_KEEPSINGLETASK				1009
#define IDC_OPTIONS_DESCRAMBLEUSESSE2			1010
#define IDC_OPTIONS_DESCRAMBLEBENCHMARK			1011
#define IDC_OPTIONS_DESCRAMBLECURSERVICEONLY	1012
#define IDC_OPTIONS_ENABLEEMMPROCESS			1013
#define IDC_OPTIONS_ENABLEBUFFERING				1020
#define IDC_OPTIONS_BUFFERSIZE_LABEL			1021
#define IDC_OPTIONS_BUFFERSIZE					1022
#define IDC_OPTIONS_BUFFERSIZE_UD				1023
#define IDC_OPTIONS_BUFFERPOOLPERCENTAGE_LABEL	1024
#define IDC_OPTIONS_BUFFERPOOLPERCENTAGE		1025
#define IDC_OPTIONS_BUFFERPOOLPERCENTAGE_UD		1026
#define IDC_OPTIONS_BUFFERPOOLPERCENTAGE_UNIT	1027
#define IDC_OPTIONS_BUFFERING_FIRST				IDC_OPTIONS_BUFFERSIZE_LABEL
#define IDC_OPTIONS_BUFFERING_LAST				IDC_OPTIONS_BUFFERPOOLPERCENTAGE_UNIT

#define IDC_OPTIONS_ADJUSTASPECTRESIZING		1000
#define IDC_OPTIONS_SNAPATWINDOWEDGE			1001
#define IDC_OPTIONS_PANSCANNORESIZEWINDOW		1002
#define IDC_OPTIONS_FULLSCREENCUTFRAME			1003
#define IDC_OPTIONS_MAXIMIZECUTFRAME			1004
#define IDC_OPTIONS_CLIENTEDGE					1005
#define IDC_OPTIONS_MINIMIZETOTRAY				1006
#define IDC_OPTIONS_MINIMIZEDISABLEPREVIEW		1007
//#define IDC_OPTIONS_NOTIFYEVENTNAME				1008
#define IDC_OPTIONS_RESETPANSCANEVENTCHANGE		1009
#define IDC_OPTIONS_RESTOREPLAYSTATUS			1010
#define IDC_OPTIONS_IGNOREDISPLAYSIZE			1011
#define IDC_OPTIONS_NOSCREENSAVER				1012
#define IDC_OPTIONS_NOMONITORLOWPOWER			1013
#define IDC_OPTIONS_NOMONITORLOWPOWERACTIVEONLY	1014
#define IDC_OPTIONS_SHOWLOGO					1015
#define IDC_OPTIONS_LOGOFILENAME				1016
#define IDC_OPTIONS_LOGOFILENAME_BROWSE			1017

#define IDC_OPTIONS_USEOSD						1000
#define IDC_OPTIONS_PSEUDOOSD					1001
#define IDC_OPTIONS_OSDTEXTCOLOR_LABEL			1002
#define IDC_OPTIONS_OSDTEXTCOLOR				1003
#define IDC_OPTIONS_OSDFADETIME_LABEL			1004
#define IDC_OPTIONS_OSDFADETIME					1005
#define IDC_OPTIONS_OSDFADETIME_UD				1006
#define IDC_OPTIONS_OSDFADETIME_UNIT			1007
#define IDC_OPTIONS_OSD_FIRST					IDC_OPTIONS_PSEUDOOSD
#define IDC_OPTIONS_OSD_LAST					IDC_OPTIONS_OSDFADETIME_UNIT
#define IDC_NOTIFICATIONBAR_ENABLE				1010
#define IDC_NOTIFICATIONBAR_NOTIFYEVENTNAME		1011
#define IDC_NOTIFICATIONBAR_DURATION_LABEL		1012
#define IDC_NOTIFICATIONBAR_DURATION			1013
#define IDC_NOTIFICATIONBAR_DURATION_UPDOWN		1014
#define IDC_NOTIFICATIONBAR_DURATION_UNIT		1015
#define IDC_NOTIFICATIONBAR_FONT_LABEL			1016
#define IDC_NOTIFICATIONBAR_FONT_INFO			1017
#define IDC_NOTIFICATIONBAR_FONT_CHOOSE			1018
#define IDC_NOTIFICATIONBAR_FIRST				IDC_NOTIFICATIONBAR_NOTIFYEVENTNAME
#define IDC_NOTIFICATIONBAR_LAST				IDC_NOTIFICATIONBAR_FONT_CHOOSE

#define IDC_STATUSOPTIONS_ITEMLIST				1000
#define IDC_STATUSOPTIONS_DEFAULT				1001
#define IDC_STATUSOPTIONS_FONTINFO				1010
#define IDC_STATUSOPTIONS_CHOOSEFONT			1011

#define IDC_SIDEBAR_SHOW						1000
#define IDC_SIDEBAR_SHOWTOOLTIPS				1001
#define IDC_SIDEBAR_ITEMLIST					1002
#define IDC_SIDEBAR_UP							1003
#define IDC_SIDEBAR_DOWN						1004
#define IDC_SIDEBAR_REMOVE						1005
#define IDC_SIDEBAR_DEFAULT						1006
#define IDC_SIDEBAR_COMMANDLIST					1007
#define IDC_SIDEBAR_ADD							1008
#define IDC_SIDEBAR_SEPARATOR					1009

#define IDC_PANELOPTIONS_SNAPATMAINWINDOW		1000
#define IDC_PANELOPTIONS_ATTACHTOMAINWINDOW		1001
#define IDC_PANELOPTIONS_OPACITY_TB				1002
#define IDC_PANELOPTIONS_OPACITY_EDIT			1003
#define IDC_PANELOPTIONS_OPACITY_UD				1004
#define IDC_PANELOPTIONS_FONTINFO				1005
#define IDC_PANELOPTIONS_CHOOSEFONT				1006
#define IDC_PANELOPTIONS_FIRSTTAB				1007

#define IDC_COLORSCHEME_PRESET					1000
#define IDC_COLORSCHEME_SAVE					1001
#define IDC_COLORSCHEME_DELETE					1002
#define IDC_COLORSCHEME_LIST					1003
#define IDC_COLORSCHEME_PALETTEPLACE			1004
#define IDC_COLORSCHEME_PALETTE					1005
#define IDC_COLORSCHEME_DEFAULT					1006
#define IDC_COLORSCHEME_PREVIEW					1007
#define IDC_COLORSCHEME_SELECTSAMECOLOR			1010
#define IDC_COLORSCHEME_GLOSSYGRADIENT			1011

#define IDC_OPTIONS_WHEELMODE					1000
#define IDC_OPTIONS_WHEELSHIFTMODE				1001
#define IDC_OPTIONS_WHEELCTRLMODE				1002
#define IDC_OPTIONS_WHEELCHANNELREVERSE			1003
#define IDC_OPTIONS_WHEELCHANNELDELAY			1004
#define IDC_OPTIONS_VOLUMESTEP					1005
#define IDC_OPTIONS_VOLUMESTEP_UD				1006
#define IDC_OPTIONS_DISPLAYDRAGMOVE				1007
#define IDC_OPTIONS_LEFTDOUBLECLICKCOMMAND		1010
#define IDC_OPTIONS_RIGHTCLICKCOMMAND			1011
#define IDC_OPTIONS_MIDDLECLICKCOMMAND			1012
#define IDC_OPTIONS_MOUSECOMMAND_FIRST			IDC_OPTIONS_LEFTDOUBLECLICKCOMMAND
#define IDC_OPTIONS_MOUSECOMMAND_LAST			IDC_OPTIONS_MIDDLECLICKCOMMAND

#define IDC_ACCELERATOR_LIST					1000
#define IDC_ACCELERATOR_KEY						1001
#define IDC_ACCELERATOR_SHIFT					1002
#define IDC_ACCELERATOR_CONTROL					1003
#define IDC_ACCELERATOR_ALT						1004
#define IDC_ACCELERATOR_GLOBAL					1005
#define IDC_ACCELERATOR_APPCOMMAND				1006
#define IDC_ACCELERATOR_DEFAULT					1007
#define IDC_OPTIONS_CHANGECH_FUNCTION			1010
#define IDC_OPTIONS_CHANGECH_DIGIT				1011
#define IDC_OPTIONS_CHANGECH_NUMPAD				1012

#define IDC_OPTIONS_USEREMOTECONTROLLER			1000
#define IDC_OPTIONS_REMOTECONTROLLERACTIVEONLY	1001
#define IDC_HDUSCONTROLLER_ASSIGN				1002
#define IDC_HDUSCONTROLLER_COMMAND				1003
#define IDC_HDUSCONTROLLER_DEFAULT				1004

#define IDC_DRIVEROPTIONS_DRIVERLIST			1000
#define IDC_DRIVEROPTIONS_INITCHANNEL_GROUP		1001
#define IDC_DRIVEROPTIONS_INITCHANNEL_NONE		1002
#define IDC_DRIVEROPTIONS_INITCHANNEL_LAST		1003
#define IDC_DRIVEROPTIONS_INITCHANNEL_CUSTOM	1004
#define IDC_DRIVEROPTIONS_INITCHANNEL_SPACE		1005
#define IDC_DRIVEROPTIONS_INITCHANNEL_CHANNEL	1006
#define IDC_DRIVEROPTIONS_DESCRAMBLEDRIVER		1007
#define IDC_DRIVEROPTIONS_NOSIGNALLEVEL			1008
#define IDC_DRIVEROPTIONS_FIRST					IDC_DRIVEROPTIONS_INITCHANNEL_GROUP
#define IDC_DRIVEROPTIONS_LAST					IDC_DRIVEROPTIONS_NOSIGNALLEVEL

#define IDC_AUDIOOPTIONS_DEVICE					1000
#define IDC_AUDIOOPTIONS_RESTOREMUTE			1001
#define IDC_AUDIOOPTIONS_DOWNMIXSURROUND		1002
#define IDC_OPTIONS_MINTIMERRESOLUTION			1010
#define IDC_OPTIONS_ADJUSTAUDIOSTREAMTIME		1011
#define IDC_OPTIONS_USEDEMUXERCLOCK				1012

#define IDC_RECORDOPTIONS_SAVEFOLDER			1000
#define IDC_RECORDOPTIONS_SAVEFOLDER_BROWSE		1001
#define IDC_RECORDOPTIONS_FILENAME				1002
#define IDC_RECORDOPTIONS_FILENAMEFORMAT		1003
#define IDC_RECORDOPTIONS_FILENAMEPREVIEW		1004
#define IDC_RECORDOPTIONS_CONFIRMCHANNELCHANGE	1010
#define IDC_RECORDOPTIONS_CONFIRMEXIT			1011
#define IDC_RECORDOPTIONS_CONFIRMSTOP			1012
#define IDC_RECORDOPTIONS_CURSERVICEONLY		1013
#define IDC_RECORDOPTIONS_SAVESUBTITLE			1014
#define IDC_RECORDOPTIONS_SAVEDATACARROUSEL		1015
#define IDC_RECORDOPTIONS_DESCRAMBLECURSERVICEONLY	1016
#define IDC_RECORDOPTIONS_BUFFERSIZE			1017
#define IDC_RECORDOPTIONS_BUFFERSIZE_UD			1018

#define IDC_CAPTUREOPTIONS_SAVEFOLDER			1000
#define IDC_CAPTUREOPTIONS_SAVEFOLDER_BROWSE	1001
#define IDC_CAPTUREOPTIONS_FILENAME				1002
#define IDC_CAPTUREOPTIONS_SIZE					1003
#define IDC_CAPTUREOPTIONS_FORMAT				1004
#define IDC_CAPTUREOPTIONS_JPEGQUALITY_TB		1005
#define IDC_CAPTUREOPTIONS_JPEGQUALITY_EDIT		1006
#define IDC_CAPTUREOPTIONS_JPEGQUALITY_UD		1007
#define IDC_CAPTUREOPTIONS_PNGLEVEL_TB			1008
#define IDC_CAPTUREOPTIONS_PNGLEVEL_EDIT		1009
#define IDC_CAPTUREOPTIONS_PNGLEVEL_UD			1010
#define IDC_CAPTUREOPTIONS_ICONSAVEFILE			1011
#define IDC_CAPTUREOPTIONS_SETCOMMENT			1012

#define IDC_CHANNELSCAN_SPACE_LABEL				1000
#define IDC_CHANNELSCAN_SPACE					1001
#define IDC_CHANNELSCAN_SCANSERVICE				1002
#define IDC_CHANNELSCAN_IGNORESIGNALLEVEL		1003
#define IDC_CHANNELSCAN_CHANNELLIST				1004
#define IDC_CHANNELSCAN_CHANNELLIST_LABEL		1005
#define IDC_CHANNELSCAN_LOADPRESET				1006
#define IDC_CHANNELSCAN_START					1007
#define IDC_CHANNELSCAN_PROPERTIES				1010
#define IDC_CHANNELSCAN_DELETE					1011
#define IDC_CHANNELSCAN_FIRST					IDC_CHANNELSCAN_SPACE_LABEL
#define IDC_CHANNELSCAN_LAST					IDC_CHANNELSCAN_START

#define IDC_CHANNELSCAN_INFO					1000
#define IDC_CHANNELSCAN_PROGRESS				1001
#define IDC_CHANNELSCAN_LEVEL					1002

#define IDC_CHANNELPROP_NAME					1000
#define IDC_CHANNELPROP_CONTROLKEY				1001
#define IDC_CHANNELPROP_TSID					1002
#define IDC_CHANNELPROP_SERVICEID				1003
#define IDC_CHANNELPROP_TUNINGSPACE				1004
#define IDC_CHANNELPROP_CHANNELINDEX			1005

#define IDC_EPGOPTIONS_USEEPGDATACAP			1000
#define IDC_EPGOPTIONS_EPGDATACAPDLLPATH		1001
#define IDC_EPGOPTIONS_EPGDATACAPDLLPATH_BROWSE	1002
#define IDC_EPGOPTIONS_SAVEEPGFILE				1003
#define IDC_EPGOPTIONS_EPGFILENAME_LABEL		1004
#define IDC_EPGOPTIONS_EPGFILENAME				1005
#define IDC_EPGOPTIONS_EPGFILENAME_BROWSE		1006
#define IDC_EPGOPTIONS_UPDATEWHENSTANDBY		1007
#define IDC_EPGOPTIONS_USEEPGDATA				1008
#define IDC_EPGOPTIONS_EPGDATAFOLDER_LABEL		1009
#define IDC_EPGOPTIONS_EPGDATAFOLDER			1010
#define IDC_EPGOPTIONS_EPGDATAFOLDER_BROWSE		1011

#define IDC_PROGRAMGUIDEOPTIONS_VIEWHOURS		1000
#define IDC_PROGRAMGUIDEOPTIONS_VIEWHOURS_UD	1001
#define IDC_PROGRAMGUIDEOPTIONS_CHANNELWIDTH	1002
#define IDC_PROGRAMGUIDEOPTIONS_CHANNELWIDTH_UD	1003
#define IDC_PROGRAMGUIDEOPTIONS_LINESPERHOUR	1004
#define IDC_PROGRAMGUIDEOPTIONS_LINESPERHOUR_UD	1005
#define IDC_PROGRAMGUIDEOPTIONS_WHEELSCROLLLINES	1006
#define IDC_PROGRAMGUIDEOPTIONS_WHEELSCROLLLINES_UD	1007
#define IDC_PROGRAMGUIDEOPTIONS_FONTINFO		1008
#define IDC_PROGRAMGUIDEOPTIONS_CHOOSEFONT		1009
#define IDC_PROGRAMGUIDETOOL_LIST				1010
#define IDC_PROGRAMGUIDETOOL_ADD				1011
#define IDC_PROGRAMGUIDETOOL_EDIT				1012
#define IDC_PROGRAMGUIDETOOL_UP					1013
#define IDC_PROGRAMGUIDETOOL_DOWN				1014
#define IDC_PROGRAMGUIDETOOL_REMOVE				1015
#define IDC_PROGRAMGUIDETOOL_REMOVEALL			1016
#define IDC_PROGRAMGUIDETOOL_HELP				1020

#define IDC_PROGRAMGUIDETOOL_NAME				1000
#define IDC_PROGRAMGUIDETOOL_COMMAND			1001
#define IDC_PROGRAMGUIDETOOL_BROWSE				1002
#define IDC_PROGRAMGUIDETOOL_PARAMETER			1003

#define IDC_PLUGIN_LIST							1000
#define IDC_PLUGIN_SETTINGS						1001
#define IDC_PLUGIN_UNLOAD						1002

#define IDC_NETWORKREMOCON_USE					1000
#define IDC_NETWORKREMOCON_ADDRESS				1001
#define IDC_NETWORKREMOCON_PORT					1002
#define IDC_NETWORKREMOCON_CHANNELFILE			1003

#define IDC_LOG_LIST							1000
#define IDC_LOG_OUTPUTTOFILE					1001
#define IDC_LOG_CLEAR							1002
#define IDC_LOG_SAVE							1003

#define IDC_RECORD_FILENAME_LABEL			1000
#define IDC_RECORD_FILENAME					1001
#define IDC_RECORD_FILENAME_BROWSE			1002
#define IDC_RECORD_FILENAMEFORMAT			1003
#define IDC_RECORD_FILENAMEPREVIEW_LABEL	1004
#define IDC_RECORD_FILENAMEPREVIEW			1005
/*
#define IDC_RECORD_FILEEXISTS_LABEL			1003
#define IDC_RECORD_FILEEXISTS				1004
*/
#define IDC_RECORD_STARTTIME				1010
#define IDC_RECORD_START_NOW				1011
#define IDC_RECORD_START_DATETIME			1012
#define IDC_RECORD_START_DELAY				1013
#define IDC_RECORD_STARTTIME_TIME			1014
#define IDC_RECORD_STARTTIME_HOUR			1015
#define IDC_RECORD_STARTTIME_HOUR_UD		1016
#define IDC_RECORD_STARTTIME_HOUR_LABEL		1017
#define IDC_RECORD_STARTTIME_MINUTE			1018
#define IDC_RECORD_STARTTIME_MINUTE_UD		1019
#define IDC_RECORD_STARTTIME_MINUTE_LABEL	1020
#define IDC_RECORD_STARTTIME_SECOND			1021
#define IDC_RECORD_STARTTIME_SECOND_UD		1022
#define IDC_RECORD_STARTTIME_SECOND_LABEL	1023
#define IDC_RECORD_STOPSPECTIME				1030
#define IDC_RECORD_STOPDATETIME				1031
#define IDC_RECORD_STOPREMAINTIME			1032
#define IDC_RECORD_STOPTIME_TIME			1033
#define IDC_RECORD_STOPTIME_HOUR			1034
#define IDC_RECORD_STOPTIME_HOUR_UD			1035
#define IDC_RECORD_STOPTIME_HOUR_LABEL		1036
#define IDC_RECORD_STOPTIME_MINUTE			1037
#define IDC_RECORD_STOPTIME_MINUTE_UD		1038
#define IDC_RECORD_STOPTIME_MINUTE_LABEL	1039
#define IDC_RECORD_STOPTIME_SECOND			1040
#define IDC_RECORD_STOPTIME_SECOND_UD		1041
#define IDC_RECORD_STOPTIME_SECOND_LABEL	1042
#define IDC_RECORD_CURSERVICEONLY			1043
#define IDC_RECORD_SAVESUBTITLE				1044
#define IDC_RECORD_SAVEDATACARROUSEL		1045
#define IDC_RECORD_CANCEL					1050

/*
#define IDC_RECORDSTOPTIME_ENABLE		1000
#define IDC_RECORDSTOPTIME_TIME			1001
*/

#define IDC_STREAMINFO_STREAM				1000
#define IDC_STREAMINFO_NETWORK				1001
#define IDC_STREAMINFO_SERVICE				1002
#define IDC_STREAMINFO_UPDATE				1003
#define IDC_STREAMINFO_COPY					1004
