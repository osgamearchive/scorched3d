# General definitions
RM = rm -f
CC = gcc
CPP = g++
DEFINES = -DNO_FLOAT_MATH -D_NO_SERVER_ASE_ -DdDOUBLE
DEFINESscorched = $(DEFINES) -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -D_REENTRANT -DGTK_NO_CHECK_CASTS
INCLUDE = -I../src -I../src/porting -I/opt/wxGTK-2.4.1/lib/wx/include/gtk-2.4 -I/opt/wxGTK-2.4.1/include -I/usr/openwin/include -I/opt/SDL/include -I/usr/cisco/packages/gtk+/gtk+-1.2.10/include/gtk-1.2 -I/usr/cisco/packages/glib/glib-1.2.10/include/glib-1.2
CCFLAGS = $(INCLUDE) -g -O2 -c -pthreads
LINKFLAGS = -g -O2 -v 
LINKLIBS = -pthreads -lGL -lGLU -lSDL -lXm -lXpm -lXmu -lXext -lXt -lSM -lICE -lX11 -ljpeg -lz -lposix4 -lnsl -lsocket -ldl -lm -lSDL_mixer -lSDL_net -lpthread -L/lib/X11 -R/lib/X11 -L/opt/SDL/lib -R/opt/SDL/lib /opt/wxGTK-2.4.1/lib/libwx_gtk-2.4.so.0.0.0 -L/usr/cisco/packages/gtk+/gtk+-1.2.10/lib -L/usr/cisco/packages/glib/glib-1.2.10/lib -lglib -lgthread -lgtk -lgdk

all : \
	scorched

scorched : \
	../src/scorched/Release/scorchedg

clean : \
	cleanscorched 

# Libraries for scorched
SCORCHEDTANKGRAPHOBJECTS = \
	../src/scorched/Release/srctankgraphGLWTankViewerscorched.o \
	../src/scorched/Release/srctankgraphTankMenusscorched.o \
	../src/scorched/Release/srctankgraphTankMeshscorched.o \
	../src/scorched/Release/srctankgraphTankModelscorched.o \
	../src/scorched/Release/srctankgraphTankModelRendererscorched.o \
	../src/scorched/Release/srctankgraphTankModelStorescorched.o \
	../src/scorched/Release/srctankgraphTankRendererscorched.o \
	../src/scorched/Release/srctankgraphTracerStorescorched.o

SCORCHEDDIALOGSOBJECTS = \
	../src/scorched/Release/srcdialogsAutoDefenseDialogscorched.o \
	../src/scorched/Release/srcdialogsBackdropDialogscorched.o \
	../src/scorched/Release/srcdialogsBuyAccessoryDialogscorched.o \
	../src/scorched/Release/srcdialogsConnectDialogscorched.o \
	../src/scorched/Release/srcdialogsHelpDialogscorched.o \
	../src/scorched/Release/srcdialogsKibitzingDialogscorched.o \
	../src/scorched/Release/srcdialogsKillDialogscorched.o \
	../src/scorched/Release/srcdialogsMainMenuDialogscorched.o \
	../src/scorched/Release/srcdialogsPlanViewDialogscorched.o \
	../src/scorched/Release/srcdialogsPlayerDialogscorched.o \
	../src/scorched/Release/srcdialogsPlayersDialogscorched.o \
	../src/scorched/Release/srcdialogsProgressDialogscorched.o \
	../src/scorched/Release/srcdialogsQuitDialogscorched.o \
	../src/scorched/Release/srcdialogsScoreDialogscorched.o \
	../src/scorched/Release/srcdialogsTalkDialogscorched.o \
	../src/scorched/Release/srcdialogsTankDialogscorched.o \
	../src/scorched/Release/srcdialogsWindDialogscorched.o

SCORCHEDGLEXTOBJECTS = \
	../src/scorched/Release/srcGLEXTGLBitmapscorched.o \
	../src/scorched/Release/srcGLEXTGLBitmapItteratorscorched.o \
	../src/scorched/Release/srcGLEXTGLBitmapModifierscorched.o \
	../src/scorched/Release/srcGLEXTGLCamerascorched.o \
	../src/scorched/Release/srcGLEXTGLCameraFrustumscorched.o \
	../src/scorched/Release/srcGLEXTGLConsolescorched.o \
	../src/scorched/Release/srcGLEXTGLConsoleFileReaderscorched.o \
	../src/scorched/Release/srcGLEXTGLConsoleLinesscorched.o \
	../src/scorched/Release/srcGLEXTGLConsoleMethodsscorched.o \
	../src/scorched/Release/srcGLEXTGLConsoleRulescorched.o \
	../src/scorched/Release/srcGLEXTGLConsoleRuleFnscorched.o \
	../src/scorched/Release/srcGLEXTGLConsoleRuleFnIAdapterscorched.o \
	../src/scorched/Release/srcGLEXTGLConsoleRuleMethodscorched.o \
	../src/scorched/Release/srcGLEXTGLConsoleRuleMethodIAdapterscorched.o \
	../src/scorched/Release/srcGLEXTGLConsoleRulesscorched.o \
	../src/scorched/Release/srcGLEXTGLFont2dscorched.o \
	../src/scorched/Release/srcGLEXTGLFontBannerscorched.o \
	../src/scorched/Release/srcGLEXTGLImagescorched.o \
	../src/scorched/Release/srcGLEXTGLLenseFlarescorched.o \
	../src/scorched/Release/srcGLEXTGLLuminancescorched.o \
	../src/scorched/Release/srcGLEXTGLMenuscorched.o \
	../src/scorched/Release/srcGLEXTGLMenuEntryscorched.o \
	../src/scorched/Release/srcGLEXTGLMenuIscorched.o \
	../src/scorched/Release/srcGLEXTGLStatescorched.o \
	../src/scorched/Release/srcGLEXTGLStateExtensionscorched.o \
	../src/scorched/Release/srcGLEXTGLTexturescorched.o \
	../src/scorched/Release/srcGLEXTGLTextureCubeMapscorched.o \
	../src/scorched/Release/srcGLEXTGLTextureSetscorched.o \
	../src/scorched/Release/srcGLEXTGLVarscorched.o \
	../src/scorched/Release/srcGLEXTGLVertexArrayscorched.o \
	../src/scorched/Release/srcGLEXTGLVertexTexArrayscorched.o

SCORCHED3DSPARSEOBJECTS = \
	../src/scorched/Release/src3dsparseASEArrayFactscorched.o \
	../src/scorched/Release/src3dsparseASEFilescorched.o \
	../src/scorched/Release/src3dsparseASEModelscorched.o \
	../src/scorched/Release/src3dsparseASEStorescorched.o \
	../src/scorched/Release/src3dsparseMeshLODscorched.o \
	../src/scorched/Release/src3dsparseMeshLODTriscorched.o \
	../src/scorched/Release/src3dsparseMeshLODVectorscorched.o \
	../src/scorched/Release/src3dsparseaseFile.lexscorched.o \
	../src/scorched/Release/src3dsparseaseFile.tabscorched.o

SCORCHEDCOMMONOBJECTS = \
	../src/scorched/Release/srccommonARGParserscorched.o \
	../src/scorched/Release/srccommonDefinesscorched.o \
	../src/scorched/Release/srccommonDisplayscorched.o \
	../src/scorched/Release/srccommonFacescorched.o \
	../src/scorched/Release/srccommonFileLinesscorched.o \
	../src/scorched/Release/srccommonFileListscorched.o \
	../src/scorched/Release/srccommonGammascorched.o \
	../src/scorched/Release/srccommonKeyboardscorched.o \
	../src/scorched/Release/srccommonLinescorched.o \
	../src/scorched/Release/srccommonLoggerscorched.o \
	../src/scorched/Release/srccommonMousescorched.o \
	../src/scorched/Release/srccommonOptionEntryscorched.o \
	../src/scorched/Release/srccommonOptionsDisplayscorched.o \
	../src/scorched/Release/srccommonOptionsGamescorched.o \
	../src/scorched/Release/srccommonOptionsParamscorched.o \
	../src/scorched/Release/srccommonOptionsTransientscorched.o \
	../src/scorched/Release/srccommonProgressCounterscorched.o \
	../src/scorched/Release/srccommonRandomGeneratorscorched.o \
	../src/scorched/Release/srccommonRegistryscorched.o \
	../src/scorched/Release/srccommonResourceFilescorched.o \
	../src/scorched/Release/srccommonResourcesscorched.o \
	../src/scorched/Release/srccommonSoundscorched.o \
	../src/scorched/Release/srccommonSoundBufferscorched.o \
	../src/scorched/Release/srccommonSoundStorescorched.o \
	../src/scorched/Release/srccommonTimerscorched.o \
	../src/scorched/Release/srccommonTrianglescorched.o \
	../src/scorched/Release/srccommonVectorscorched.o \
	../src/scorched/Release/srccommonVectorLibscorched.o \
	../src/scorched/Release/srccommonWindowManagerscorched.o

SCORCHEDGLWOBJECTS = \
	../src/scorched/Release/srcGLWGLWButtonscorched.o \
	../src/scorched/Release/srcGLWGLWCheckBoxscorched.o \
	../src/scorched/Release/srcGLWGLWDropDownscorched.o \
	../src/scorched/Release/srcGLWGLWFileViewscorched.o \
	../src/scorched/Release/srcGLWGLWFlagscorched.o \
	../src/scorched/Release/srcGLWGLWFontscorched.o \
	../src/scorched/Release/srcGLWGLWLabelscorched.o \
	../src/scorched/Release/srcGLWGLWListViewscorched.o \
	../src/scorched/Release/srcGLWGLWPanelscorched.o \
	../src/scorched/Release/srcGLWGLWProgressscorched.o \
	../src/scorched/Release/srcGLWGLWPushButtonscorched.o \
	../src/scorched/Release/srcGLWGLWScrollButtonscorched.o \
	../src/scorched/Release/srcGLWGLWScrollHscorched.o \
	../src/scorched/Release/srcGLWGLWScrollPanelscorched.o \
	../src/scorched/Release/srcGLWGLWScrollWscorched.o \
	../src/scorched/Release/srcGLWGLWSpinBoxscorched.o \
	../src/scorched/Release/srcGLWGLWTabscorched.o \
	../src/scorched/Release/srcGLWGLWTextBoxscorched.o \
	../src/scorched/Release/srcGLWGLWTextButtonscorched.o \
	../src/scorched/Release/srcGLWGLWVisiblePanelscorched.o \
	../src/scorched/Release/srcGLWGLWVisibleWidgetscorched.o \
	../src/scorched/Release/srcGLWGLWWindowscorched.o \
	../src/scorched/Release/srcGLWGLWidgetscorched.o

SCORCHEDSPRITESOBJECTS = \
	../src/scorched/Release/srcspritesDebrisActionRendererscorched.o \
	../src/scorched/Release/srcspritesExplosionNukeRendererscorched.o \
	../src/scorched/Release/srcspritesExplosionRendererscorched.o \
	../src/scorched/Release/srcspritesExplosionTexturesscorched.o \
	../src/scorched/Release/srcspritesFlareActionRendererscorched.o \
	../src/scorched/Release/srcspritesSmokeActionRendererscorched.o \
	../src/scorched/Release/srcspritesTalkRendererscorched.o \
	../src/scorched/Release/srcspritesTankDeadRendererscorched.o \
	../src/scorched/Release/srcspritesTextActionRendererscorched.o \
	../src/scorched/Release/srcspritesTracerActionRendererscorched.o \
	../src/scorched/Release/srcspritesWallActionRendererscorched.o

SCORCHEDLANDSCAPEOBJECTS = \
	../src/scorched/Release/srclandscapeCloudSimscorched.o \
	../src/scorched/Release/srclandscapeDeformLandscapescorched.o \
	../src/scorched/Release/srclandscapeDeformTexturesscorched.o \
	../src/scorched/Release/srclandscapeGlobalHMapscorched.o \
	../src/scorched/Release/srclandscapeHeightMapscorched.o \
	../src/scorched/Release/srclandscapeHeightMapCollisionscorched.o \
	../src/scorched/Release/srclandscapeHeightMapLoaderscorched.o \
	../src/scorched/Release/srclandscapeHeightMapModifierscorched.o \
	../src/scorched/Release/srclandscapeHeightMapRendererscorched.o \
	../src/scorched/Release/srclandscapeHeightMapSurroundscorched.o \
	../src/scorched/Release/srclandscapeHemispherescorched.o \
	../src/scorched/Release/srclandscapeInfoMapscorched.o \
	../src/scorched/Release/srclandscapeLanscapescorched.o \
	../src/scorched/Release/srclandscapeMovementMapscorched.o \
	../src/scorched/Release/srclandscapeNapalmMapscorched.o \
	../src/scorched/Release/srclandscapePatchscorched.o \
	../src/scorched/Release/srclandscapePatchGridscorched.o \
	../src/scorched/Release/srclandscapePatchSidescorched.o \
	../src/scorched/Release/srclandscapePatchVarscorched.o \
	../src/scorched/Release/srclandscapeShadowMapscorched.o \
	../src/scorched/Release/srclandscapeSmokescorched.o \
	../src/scorched/Release/srclandscapeSmokeChainscorched.o \
	../src/scorched/Release/srclandscapeSunscorched.o \
	../src/scorched/Release/srclandscapeSurroundscorched.o \
	../src/scorched/Release/srclandscapeSurroundDefsscorched.o \
	../src/scorched/Release/srclandscapeTriNodescorched.o \
	../src/scorched/Release/srclandscapeTriNodePoolscorched.o \
	../src/scorched/Release/srclandscapeVariancescorched.o \
	../src/scorched/Release/srclandscapeWallscorched.o \
	../src/scorched/Release/srclandscapeWaterMapscorched.o \
	../src/scorched/Release/srclandscapeWaterMapModifierscorched.o \
	../src/scorched/Release/srclandscapeWaterMapPointsscorched.o \
	../src/scorched/Release/srclandscapeWaterMapSurroundscorched.o

SCORCHEDSCORCHEDOBJECTS = \
	../src/scorched/Release/srcscorchedCreateDialogsscorched.o \
	../src/scorched/Release/srcscorchedDisplayDialogscorched.o \
	../src/scorched/Release/srcscorchedMainDialogscorched.o \
	../src/scorched/Release/srcscorchedNetDialogscorched.o \
	../src/scorched/Release/srcscorchedServerDialogscorched.o \
	../src/scorched/Release/srcscorchedServerSDialogscorched.o \
	../src/scorched/Release/srcscorchedSettingsDialogscorched.o \
	../src/scorched/Release/srcscorchedSingleDialogscorched.o \
	../src/scorched/Release/srcscorchedmainscorched.o

SCORCHEDTANKOBJECTS = \
	../src/scorched/Release/srctankTankscorched.o \
	../src/scorched/Release/srctankTankAIscorched.o \
	../src/scorched/Release/srctankTankAILogicscorched.o \
	../src/scorched/Release/srctankTankAccessoriesscorched.o \
	../src/scorched/Release/srctankTankAutoDefensescorched.o \
	../src/scorched/Release/srctankTankBatteriesscorched.o \
	../src/scorched/Release/srctankTankColorGeneratorscorched.o \
	../src/scorched/Release/srctankTankContainerscorched.o \
	../src/scorched/Release/srctankTankControllerscorched.o \
	../src/scorched/Release/srctankTankFuelscorched.o \
	../src/scorched/Release/srctankTankLibscorched.o \
	../src/scorched/Release/srctankTankModelIdscorched.o \
	../src/scorched/Release/srctankTankParachutesscorched.o \
	../src/scorched/Release/srctankTankPhysicsscorched.o \
	../src/scorched/Release/srctankTankScorescorched.o \
	../src/scorched/Release/srctankTankShieldsscorched.o \
	../src/scorched/Release/srctankTankSortscorched.o \
	../src/scorched/Release/srctankTankStartscorched.o \
	../src/scorched/Release/srctankTankStatescorched.o \
	../src/scorched/Release/srctankTankWeaponscorched.o

SCORCHEDXMLOBJECTS = \
	../src/scorched/Release/srcXMLXMLFilescorched.o \
	../src/scorched/Release/srcXMLXMLParserscorched.o \
	../src/scorched/Release/srcXMLxmlparsescorched.o \
	../src/scorched/Release/srcXMLxmlrolescorched.o \
	../src/scorched/Release/srcXMLxmltokscorched.o

SCORCHEDZLIBOBJECTS = \
	../src/scorched/Release/srczlibadler32scorched.o \
	../src/scorched/Release/srczlibcompressscorched.o \
	../src/scorched/Release/srczlibcrc32scorched.o \
	../src/scorched/Release/srczlibdeflatescorched.o \
	../src/scorched/Release/srczlibgzioscorched.o \
	../src/scorched/Release/srczlibinfblockscorched.o \
	../src/scorched/Release/srczlibinfcodesscorched.o \
	../src/scorched/Release/srczlibinffastscorched.o \
	../src/scorched/Release/srczlibinflatescorched.o \
	../src/scorched/Release/srczlibinftreesscorched.o \
	../src/scorched/Release/srczlibinfutilscorched.o \
	../src/scorched/Release/srczlibtreesscorched.o \
	../src/scorched/Release/srczlibuncomprscorched.o \
	../src/scorched/Release/srczlibzutilscorched.o

SCORCHEDACTIONSOBJECTS = \
	../src/scorched/Release/srcactionsExplosionscorched.o \
	../src/scorched/Release/srcactionsNapalmscorched.o \
	../src/scorched/Release/srcactionsShieldHitscorched.o \
	../src/scorched/Release/srcactionsShotProjectilescorched.o \
	../src/scorched/Release/srcactionsShotProjectileClodscorched.o \
	../src/scorched/Release/srcactionsShotProjectileExplosionscorched.o \
	../src/scorched/Release/srcactionsShotProjectileFunkyscorched.o \
	../src/scorched/Release/srcactionsShotProjectileMirvscorched.o \
	../src/scorched/Release/srcactionsShotProjectileNapalmscorched.o \
	../src/scorched/Release/srcactionsShotProjectileRiotscorched.o \
	../src/scorched/Release/srcactionsShotProjectileTracerscorched.o \
	../src/scorched/Release/srcactionsShowScorescorched.o \
	../src/scorched/Release/srcactionsSpriteProjectilescorched.o \
	../src/scorched/Release/srcactionsTankDamagescorched.o \
	../src/scorched/Release/srcactionsTankDeadscorched.o \
	../src/scorched/Release/srcactionsTankFallingscorched.o \
	../src/scorched/Release/srcactionsTankFiredscorched.o \
	../src/scorched/Release/srcactionsTankMovescorched.o \
	../src/scorched/Release/srcactionsTankMovementscorched.o \
	../src/scorched/Release/srcactionsTankResignscorched.o \
	../src/scorched/Release/srcactionsTankSayscorched.o \
	../src/scorched/Release/srcactionsTankScoredscorched.o \
	../src/scorched/Release/srcactionsWallHitscorched.o

SCORCHEDCOMSOBJECTS = \
	../src/scorched/Release/srccomsComsActionsMessagescorched.o \
	../src/scorched/Release/srccomsComsAddPlayerMessagescorched.o \
	../src/scorched/Release/srccomsComsBuyAccessoryMessagescorched.o \
	../src/scorched/Release/srccomsComsConnectAcceptMessagescorched.o \
	../src/scorched/Release/srccomsComsConnectMessagescorched.o \
	../src/scorched/Release/srccomsComsDefenseMessagescorched.o \
	../src/scorched/Release/srccomsComsGameStoppedMessagescorched.o \
	../src/scorched/Release/srccomsComsLevelMessagescorched.o \
	../src/scorched/Release/srccomsComsMessagescorched.o \
	../src/scorched/Release/srccomsComsMessageHandlerscorched.o \
	../src/scorched/Release/srccomsComsMessageSenderscorched.o \
	../src/scorched/Release/srccomsComsNewGameMessagescorched.o \
	../src/scorched/Release/srccomsComsPlayedMoveMessagescorched.o \
	../src/scorched/Release/srccomsComsPlayerReadyMessagescorched.o \
	../src/scorched/Release/srccomsComsPlayerStateMessagescorched.o \
	../src/scorched/Release/srccomsComsRmPlayerMessagescorched.o \
	../src/scorched/Release/srccomsComsScoreMessagescorched.o \
	../src/scorched/Release/srccomsComsStartGameMessagescorched.o \
	../src/scorched/Release/srccomsComsTextMessagescorched.o \
	../src/scorched/Release/srccomsNetBufferscorched.o \
	../src/scorched/Release/srccomsNetBufferUtilscorched.o \
	../src/scorched/Release/srccomsNetClientscorched.o \
	../src/scorched/Release/srccomsNetMessagescorched.o \
	../src/scorched/Release/srccomsNetMessageHandlerscorched.o \
	../src/scorched/Release/srccomsNetMessagePoolscorched.o \
	../src/scorched/Release/srccomsNetServerscorched.o

SCORCHEDENGINEOBJECTS = \
	../src/scorched/Release/srcengineActionscorched.o \
	../src/scorched/Release/srcengineActionBufferscorched.o \
	../src/scorched/Release/srcengineActionControllerscorched.o \
	../src/scorched/Release/srcengineActionMetascorched.o \
	../src/scorched/Release/srcengineFrameTimerscorched.o \
	../src/scorched/Release/srcengineGameStatescorched.o \
	../src/scorched/Release/srcengineGameStateIscorched.o \
	../src/scorched/Release/srcengineGameStateStimulusIscorched.o \
	../src/scorched/Release/srcengineMainLoopscorched.o \
	../src/scorched/Release/srcengineMainLoopIscorched.o \
	../src/scorched/Release/srcenginePhysicsEnginescorched.o \
	../src/scorched/Release/srcenginePhysicsParticlescorched.o \
	../src/scorched/Release/srcenginePhysicsParticleObjectscorched.o \
	../src/scorched/Release/srcengineScorchedCollisionHandlerscorched.o \
	../src/scorched/Release/srcengineScorchedCollisionIdsscorched.o

SCORCHEDSERVEROBJECTS = \
	../src/scorched/Release/srcserverScorchedASEscorched.o \
	../src/scorched/Release/srcserverServerBuyAccessoryHandlerscorched.o \
	../src/scorched/Release/srcserverServerConnectHandlerscorched.o \
	../src/scorched/Release/srcserverServerDefenseHandlerscorched.o \
	../src/scorched/Release/srcserverServerMainscorched.o \
	../src/scorched/Release/srcserverServerMessageHandlerscorched.o \
	../src/scorched/Release/srcserverServerNewGameStatescorched.o \
	../src/scorched/Release/srcserverServerNextRoundStatescorched.o \
	../src/scorched/Release/srcserverServerPlayedMoveHandlerscorched.o \
	../src/scorched/Release/srcserverServerPlayerReadyHandlerscorched.o \
	../src/scorched/Release/srcserverServerPlayingStatescorched.o \
	../src/scorched/Release/srcserverServerScoreStatescorched.o \
	../src/scorched/Release/srcserverServerShotHolderscorched.o \
	../src/scorched/Release/srcserverServerShotStatescorched.o \
	../src/scorched/Release/srcserverServerStatescorched.o \
	../src/scorched/Release/srcserverServerTextHandlerscorched.o \
	../src/scorched/Release/srcserverServerTooFewPlayersStimulusscorched.o \
	../src/scorched/Release/srcserverServerWaitingForPlayersStatescorched.o

SCORCHEDTANKAIOBJECTS = \
	../src/scorched/Release/srctankaiTankAIAdderscorched.o \
	../src/scorched/Release/srctankaiTankAIComputerscorched.o \
	../src/scorched/Release/srctankaiTankAIComputerBuyerscorched.o \
	../src/scorched/Release/srctankaiTankAIComputerChooserscorched.o \
	../src/scorched/Release/srctankaiTankAIComputerCtrlscorched.o \
	../src/scorched/Release/srctankaiTankAIComputerDHTestscorched.o \
	../src/scorched/Release/srctankaiTankAIComputerMoronscorched.o \
	../src/scorched/Release/srctankaiTankAIComputerPSharkscorched.o \
	../src/scorched/Release/srctankaiTankAIComputerRandomscorched.o \
	../src/scorched/Release/srctankaiTankAIComputerShooterscorched.o \
	../src/scorched/Release/srctankaiTankAIComputerTosserscorched.o \
	../src/scorched/Release/srctankaiTankAIHumanscorched.o \
	../src/scorched/Release/srctankaiTankAIHumanCtrlscorched.o \
	../src/scorched/Release/srctankaiTankAIStorescorched.o \
	../src/scorched/Release/srctankaiTankAIStringsscorched.o

SCORCHEDODEOBJECTS = \
	../src/scorched/Release/srcodearrayscorched.o \
	../src/scorched/Release/srcodeerrorscorched.o \
	../src/scorched/Release/srcodefastdotscorched.o \
	../src/scorched/Release/srcodefastldltscorched.o \
	../src/scorched/Release/srcodefastlsolvescorched.o \
	../src/scorched/Release/srcodefastltsolvescorched.o \
	../src/scorched/Release/srcodegeomscorched.o \
	../src/scorched/Release/srcodejointscorched.o \
	../src/scorched/Release/srcodelcpscorched.o \
	../src/scorched/Release/srcodemassscorched.o \
	../src/scorched/Release/srcodematscorched.o \
	../src/scorched/Release/srcodematrixscorched.o \
	../src/scorched/Release/srcodememoryscorched.o \
	../src/scorched/Release/srcodemiscscorched.o \
	../src/scorched/Release/srcodeobstackscorched.o \
	../src/scorched/Release/srcodeodescorched.o \
	../src/scorched/Release/srcodeodemathscorched.o \
	../src/scorched/Release/srcoderotationscorched.o \
	../src/scorched/Release/srcodespacescorched.o \
	../src/scorched/Release/srcodestepscorched.o \
	../src/scorched/Release/srcodetestingscorched.o

SCORCHEDWEAPONSOBJECTS = \
	../src/scorched/Release/srcweaponsAccessoryscorched.o \
	../src/scorched/Release/srcweaponsAccessoryStorescorched.o \
	../src/scorched/Release/srcweaponsAutoDefensescorched.o \
	../src/scorched/Release/srcweaponsBatteryscorched.o \
	../src/scorched/Release/srcweaponsFuelscorched.o \
	../src/scorched/Release/srcweaponsParachutescorched.o \
	../src/scorched/Release/srcweaponsShieldscorched.o \
	../src/scorched/Release/srcweaponsShieldReflectivescorched.o \
	../src/scorched/Release/srcweaponsShieldReflectiveMagscorched.o \
	../src/scorched/Release/srcweaponsWeaponscorched.o \
	../src/scorched/Release/srcweaponsWeaponClodscorched.o \
	../src/scorched/Release/srcweaponsWeaponFunkyscorched.o \
	../src/scorched/Release/srcweaponsWeaponMirvscorched.o \
	../src/scorched/Release/srcweaponsWeaponNapalmscorched.o \
	../src/scorched/Release/srcweaponsWeaponProjectilescorched.o \
	../src/scorched/Release/srcweaponsWeaponRiotBombscorched.o \
	../src/scorched/Release/srcweaponsWeaponTracerscorched.o

SCORCHEDCLIENTOBJECTS = \
	../src/scorched/Release/srcclientClientActionsHandlerscorched.o \
	../src/scorched/Release/srcclientClientAddPlayerHandlerscorched.o \
	../src/scorched/Release/srcclientClientBuyAccessoryHandlerscorched.o \
	../src/scorched/Release/srcclientClientConnectionAcceptHandlerscorched.o \
	../src/scorched/Release/srcclientClientDefenseHandlerscorched.o \
	../src/scorched/Release/srcclientClientDialogscorched.o \
	../src/scorched/Release/srcclientClientGameStoppedHandlerscorched.o \
	../src/scorched/Release/srcclientClientMainscorched.o \
	../src/scorched/Release/srcclientClientMessageHandlerscorched.o \
	../src/scorched/Release/srcclientClientNewGameHandlerscorched.o \
	../src/scorched/Release/srcclientClientNextRoundStatescorched.o \
	../src/scorched/Release/srcclientClientRmPlayerHandlerscorched.o \
	../src/scorched/Release/srcclientClientScoreHandlerscorched.o \
	../src/scorched/Release/srcclientClientShotStatescorched.o \
	../src/scorched/Release/srcclientClientStartGameHandlerscorched.o \
	../src/scorched/Release/srcclientClientStatescorched.o \
	../src/scorched/Release/srcclientClientTextHandlerscorched.o \
	../src/scorched/Release/srcclientGLSetupscorched.o \
	../src/scorched/Release/srcclientMain2DCamerascorched.o \
	../src/scorched/Release/srcclientMainBannerscorched.o \
	../src/scorched/Release/srcclientMainCamerascorched.o \
	../src/scorched/Release/srcclientShotTimerscorched.o \
	../src/scorched/Release/srcclientSpeedChangescorched.o \
	../src/scorched/Release/srcclientWindowSetupscorched.o


../src/scorched/Release/scorchedg : \
	$(SCORCHEDTANKGRAPHOBJECTS) \
	$(SCORCHEDDIALOGSOBJECTS) \
	$(SCORCHEDGLEXTOBJECTS) \
	$(SCORCHED3DSPARSEOBJECTS) \
	$(SCORCHEDCOMMONOBJECTS) \
	$(SCORCHEDGLWOBJECTS) \
	$(SCORCHEDSPRITESOBJECTS) \
	$(SCORCHEDLANDSCAPEOBJECTS) \
	$(SCORCHEDSCORCHEDOBJECTS) \
	$(SCORCHEDTANKOBJECTS) \
	$(SCORCHEDXMLOBJECTS) \
	$(SCORCHEDZLIBOBJECTS) \
	$(SCORCHEDACTIONSOBJECTS) \
	$(SCORCHEDCOMSOBJECTS) \
	$(SCORCHEDENGINEOBJECTS) \
	$(SCORCHEDSERVEROBJECTS) \
	$(SCORCHEDTANKAIOBJECTS) \
	$(SCORCHEDODEOBJECTS) \
	$(SCORCHEDWEAPONSOBJECTS) \
	$(SCORCHEDCLIENTOBJECTS) 
	$(CPP) $(LINKFLAGS) $(LINKLIBS)\
	$(SCORCHEDTANKGRAPHOBJECTS) \
	$(SCORCHEDDIALOGSOBJECTS) \
	$(SCORCHEDGLEXTOBJECTS) \
	$(SCORCHED3DSPARSEOBJECTS) \
	$(SCORCHEDCOMMONOBJECTS) \
	$(SCORCHEDGLWOBJECTS) \
	$(SCORCHEDSPRITESOBJECTS) \
	$(SCORCHEDLANDSCAPEOBJECTS) \
	$(SCORCHEDSCORCHEDOBJECTS) \
	$(SCORCHEDTANKOBJECTS) \
	$(SCORCHEDXMLOBJECTS) \
	$(SCORCHEDZLIBOBJECTS) \
	$(SCORCHEDACTIONSOBJECTS) \
	$(SCORCHEDCOMSOBJECTS) \
	$(SCORCHEDENGINEOBJECTS) \
	$(SCORCHEDSERVEROBJECTS) \
	$(SCORCHEDTANKAIOBJECTS) \
	$(SCORCHEDODEOBJECTS) \
	$(SCORCHEDWEAPONSOBJECTS) \
	$(SCORCHEDCLIENTOBJECTS) \
	-o ../src/scorched/Release/scorchedg

cleanscorched :
	$(RM) ../src/scorched/Release/scorchedg
	$(RM) $(SCORCHEDTANKGRAPHOBJECTS)
	$(RM) $(SCORCHEDDIALOGSOBJECTS)
	$(RM) $(SCORCHEDGLEXTOBJECTS)
	$(RM) $(SCORCHED3DSPARSEOBJECTS)
	$(RM) $(SCORCHEDCOMMONOBJECTS)
	$(RM) $(SCORCHEDGLWOBJECTS)
	$(RM) $(SCORCHEDSPRITESOBJECTS)
	$(RM) $(SCORCHEDLANDSCAPEOBJECTS)
	$(RM) $(SCORCHEDSCORCHEDOBJECTS)
	$(RM) $(SCORCHEDTANKOBJECTS)
	$(RM) $(SCORCHEDXMLOBJECTS)
	$(RM) $(SCORCHEDZLIBOBJECTS)
	$(RM) $(SCORCHEDACTIONSOBJECTS)
	$(RM) $(SCORCHEDCOMSOBJECTS)
	$(RM) $(SCORCHEDENGINEOBJECTS)
	$(RM) $(SCORCHEDSERVEROBJECTS)
	$(RM) $(SCORCHEDTANKAIOBJECTS)
	$(RM) $(SCORCHEDODEOBJECTS)
	$(RM) $(SCORCHEDWEAPONSOBJECTS)
	$(RM) $(SCORCHEDCLIENTOBJECTS)

# CPP Build Rules
../src/scorched/Release/srctankgraphGLWTankViewerscorched.o : ../src/tankgraph/GLWTankViewer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankgraphGLWTankViewerscorched.o ../src/tankgraph/GLWTankViewer.cpp

../src/scorched/Release/srctankgraphTankMenusscorched.o : ../src/tankgraph/TankMenus.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankgraphTankMenusscorched.o ../src/tankgraph/TankMenus.cpp

../src/scorched/Release/srctankgraphTankMeshscorched.o : ../src/tankgraph/TankMesh.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankgraphTankMeshscorched.o ../src/tankgraph/TankMesh.cpp

../src/scorched/Release/srctankgraphTankModelscorched.o : ../src/tankgraph/TankModel.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankgraphTankModelscorched.o ../src/tankgraph/TankModel.cpp

../src/scorched/Release/srctankgraphTankModelRendererscorched.o : ../src/tankgraph/TankModelRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankgraphTankModelRendererscorched.o ../src/tankgraph/TankModelRenderer.cpp

../src/scorched/Release/srctankgraphTankModelStorescorched.o : ../src/tankgraph/TankModelStore.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankgraphTankModelStorescorched.o ../src/tankgraph/TankModelStore.cpp

../src/scorched/Release/srctankgraphTankRendererscorched.o : ../src/tankgraph/TankRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankgraphTankRendererscorched.o ../src/tankgraph/TankRenderer.cpp

../src/scorched/Release/srctankgraphTracerStorescorched.o : ../src/tankgraph/TracerStore.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankgraphTracerStorescorched.o ../src/tankgraph/TracerStore.cpp

../src/scorched/Release/srcdialogsAutoDefenseDialogscorched.o : ../src/dialogs/AutoDefenseDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsAutoDefenseDialogscorched.o ../src/dialogs/AutoDefenseDialog.cpp

../src/scorched/Release/srcdialogsBackdropDialogscorched.o : ../src/dialogs/BackdropDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsBackdropDialogscorched.o ../src/dialogs/BackdropDialog.cpp

../src/scorched/Release/srcdialogsBuyAccessoryDialogscorched.o : ../src/dialogs/BuyAccessoryDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsBuyAccessoryDialogscorched.o ../src/dialogs/BuyAccessoryDialog.cpp

../src/scorched/Release/srcdialogsConnectDialogscorched.o : ../src/dialogs/ConnectDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsConnectDialogscorched.o ../src/dialogs/ConnectDialog.cpp

../src/scorched/Release/srcdialogsHelpDialogscorched.o : ../src/dialogs/HelpDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsHelpDialogscorched.o ../src/dialogs/HelpDialog.cpp

../src/scorched/Release/srcdialogsKibitzingDialogscorched.o : ../src/dialogs/KibitzingDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsKibitzingDialogscorched.o ../src/dialogs/KibitzingDialog.cpp

../src/scorched/Release/srcdialogsKillDialogscorched.o : ../src/dialogs/KillDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsKillDialogscorched.o ../src/dialogs/KillDialog.cpp

../src/scorched/Release/srcdialogsMainMenuDialogscorched.o : ../src/dialogs/MainMenuDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsMainMenuDialogscorched.o ../src/dialogs/MainMenuDialog.cpp

../src/scorched/Release/srcdialogsPlanViewDialogscorched.o : ../src/dialogs/PlanViewDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsPlanViewDialogscorched.o ../src/dialogs/PlanViewDialog.cpp

../src/scorched/Release/srcdialogsPlayerDialogscorched.o : ../src/dialogs/PlayerDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsPlayerDialogscorched.o ../src/dialogs/PlayerDialog.cpp

../src/scorched/Release/srcdialogsPlayersDialogscorched.o : ../src/dialogs/PlayersDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsPlayersDialogscorched.o ../src/dialogs/PlayersDialog.cpp

../src/scorched/Release/srcdialogsProgressDialogscorched.o : ../src/dialogs/ProgressDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsProgressDialogscorched.o ../src/dialogs/ProgressDialog.cpp

../src/scorched/Release/srcdialogsQuitDialogscorched.o : ../src/dialogs/QuitDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsQuitDialogscorched.o ../src/dialogs/QuitDialog.cpp

../src/scorched/Release/srcdialogsScoreDialogscorched.o : ../src/dialogs/ScoreDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsScoreDialogscorched.o ../src/dialogs/ScoreDialog.cpp

../src/scorched/Release/srcdialogsTalkDialogscorched.o : ../src/dialogs/TalkDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsTalkDialogscorched.o ../src/dialogs/TalkDialog.cpp

../src/scorched/Release/srcdialogsTankDialogscorched.o : ../src/dialogs/TankDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsTankDialogscorched.o ../src/dialogs/TankDialog.cpp

../src/scorched/Release/srcdialogsWindDialogscorched.o : ../src/dialogs/WindDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcdialogsWindDialogscorched.o ../src/dialogs/WindDialog.cpp

../src/scorched/Release/srcGLEXTGLBitmapscorched.o : ../src/GLEXT/GLBitmap.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLBitmapscorched.o ../src/GLEXT/GLBitmap.cpp

../src/scorched/Release/srcGLEXTGLBitmapItteratorscorched.o : ../src/GLEXT/GLBitmapItterator.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLBitmapItteratorscorched.o ../src/GLEXT/GLBitmapItterator.cpp

../src/scorched/Release/srcGLEXTGLBitmapModifierscorched.o : ../src/GLEXT/GLBitmapModifier.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLBitmapModifierscorched.o ../src/GLEXT/GLBitmapModifier.cpp

../src/scorched/Release/srcGLEXTGLCamerascorched.o : ../src/GLEXT/GLCamera.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLCamerascorched.o ../src/GLEXT/GLCamera.cpp

../src/scorched/Release/srcGLEXTGLCameraFrustumscorched.o : ../src/GLEXT/GLCameraFrustum.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLCameraFrustumscorched.o ../src/GLEXT/GLCameraFrustum.cpp

../src/scorched/Release/srcGLEXTGLConsolescorched.o : ../src/GLEXT/GLConsole.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLConsolescorched.o ../src/GLEXT/GLConsole.cpp

../src/scorched/Release/srcGLEXTGLConsoleFileReaderscorched.o : ../src/GLEXT/GLConsoleFileReader.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLConsoleFileReaderscorched.o ../src/GLEXT/GLConsoleFileReader.cpp

../src/scorched/Release/srcGLEXTGLConsoleLinesscorched.o : ../src/GLEXT/GLConsoleLines.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLConsoleLinesscorched.o ../src/GLEXT/GLConsoleLines.cpp

../src/scorched/Release/srcGLEXTGLConsoleMethodsscorched.o : ../src/GLEXT/GLConsoleMethods.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLConsoleMethodsscorched.o ../src/GLEXT/GLConsoleMethods.cpp

../src/scorched/Release/srcGLEXTGLConsoleRulescorched.o : ../src/GLEXT/GLConsoleRule.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLConsoleRulescorched.o ../src/GLEXT/GLConsoleRule.cpp

../src/scorched/Release/srcGLEXTGLConsoleRuleFnscorched.o : ../src/GLEXT/GLConsoleRuleFn.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLConsoleRuleFnscorched.o ../src/GLEXT/GLConsoleRuleFn.cpp

../src/scorched/Release/srcGLEXTGLConsoleRuleFnIAdapterscorched.o : ../src/GLEXT/GLConsoleRuleFnIAdapter.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLConsoleRuleFnIAdapterscorched.o ../src/GLEXT/GLConsoleRuleFnIAdapter.cpp

../src/scorched/Release/srcGLEXTGLConsoleRuleMethodscorched.o : ../src/GLEXT/GLConsoleRuleMethod.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLConsoleRuleMethodscorched.o ../src/GLEXT/GLConsoleRuleMethod.cpp

../src/scorched/Release/srcGLEXTGLConsoleRuleMethodIAdapterscorched.o : ../src/GLEXT/GLConsoleRuleMethodIAdapter.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLConsoleRuleMethodIAdapterscorched.o ../src/GLEXT/GLConsoleRuleMethodIAdapter.cpp

../src/scorched/Release/srcGLEXTGLConsoleRulesscorched.o : ../src/GLEXT/GLConsoleRules.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLConsoleRulesscorched.o ../src/GLEXT/GLConsoleRules.cpp

../src/scorched/Release/srcGLEXTGLFont2dscorched.o : ../src/GLEXT/GLFont2d.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLFont2dscorched.o ../src/GLEXT/GLFont2d.cpp

../src/scorched/Release/srcGLEXTGLFontBannerscorched.o : ../src/GLEXT/GLFontBanner.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLFontBannerscorched.o ../src/GLEXT/GLFontBanner.cpp

../src/scorched/Release/srcGLEXTGLImagescorched.o : ../src/GLEXT/GLImage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLImagescorched.o ../src/GLEXT/GLImage.cpp

../src/scorched/Release/srcGLEXTGLLenseFlarescorched.o : ../src/GLEXT/GLLenseFlare.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLLenseFlarescorched.o ../src/GLEXT/GLLenseFlare.cpp

../src/scorched/Release/srcGLEXTGLLuminancescorched.o : ../src/GLEXT/GLLuminance.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLLuminancescorched.o ../src/GLEXT/GLLuminance.cpp

../src/scorched/Release/srcGLEXTGLMenuscorched.o : ../src/GLEXT/GLMenu.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLMenuscorched.o ../src/GLEXT/GLMenu.cpp

../src/scorched/Release/srcGLEXTGLMenuEntryscorched.o : ../src/GLEXT/GLMenuEntry.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLMenuEntryscorched.o ../src/GLEXT/GLMenuEntry.cpp

../src/scorched/Release/srcGLEXTGLMenuIscorched.o : ../src/GLEXT/GLMenuI.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLMenuIscorched.o ../src/GLEXT/GLMenuI.cpp

../src/scorched/Release/srcGLEXTGLStatescorched.o : ../src/GLEXT/GLState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLStatescorched.o ../src/GLEXT/GLState.cpp

../src/scorched/Release/srcGLEXTGLStateExtensionscorched.o : ../src/GLEXT/GLStateExtension.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLStateExtensionscorched.o ../src/GLEXT/GLStateExtension.cpp

../src/scorched/Release/srcGLEXTGLTexturescorched.o : ../src/GLEXT/GLTexture.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLTexturescorched.o ../src/GLEXT/GLTexture.cpp

../src/scorched/Release/srcGLEXTGLTextureCubeMapscorched.o : ../src/GLEXT/GLTextureCubeMap.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLTextureCubeMapscorched.o ../src/GLEXT/GLTextureCubeMap.cpp

../src/scorched/Release/srcGLEXTGLTextureSetscorched.o : ../src/GLEXT/GLTextureSet.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLTextureSetscorched.o ../src/GLEXT/GLTextureSet.cpp

../src/scorched/Release/srcGLEXTGLVarscorched.o : ../src/GLEXT/GLVar.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLVarscorched.o ../src/GLEXT/GLVar.cpp

../src/scorched/Release/srcGLEXTGLVertexArrayscorched.o : ../src/GLEXT/GLVertexArray.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLVertexArrayscorched.o ../src/GLEXT/GLVertexArray.cpp

../src/scorched/Release/srcGLEXTGLVertexTexArrayscorched.o : ../src/GLEXT/GLVertexTexArray.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLEXTGLVertexTexArrayscorched.o ../src/GLEXT/GLVertexTexArray.cpp

../src/scorched/Release/src3dsparseASEArrayFactscorched.o : ../src/3dsparse/ASEArrayFact.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/src3dsparseASEArrayFactscorched.o ../src/3dsparse/ASEArrayFact.cpp

../src/scorched/Release/src3dsparseASEFilescorched.o : ../src/3dsparse/ASEFile.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/src3dsparseASEFilescorched.o ../src/3dsparse/ASEFile.cpp

../src/scorched/Release/src3dsparseASEModelscorched.o : ../src/3dsparse/ASEModel.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/src3dsparseASEModelscorched.o ../src/3dsparse/ASEModel.cpp

../src/scorched/Release/src3dsparseASEStorescorched.o : ../src/3dsparse/ASEStore.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/src3dsparseASEStorescorched.o ../src/3dsparse/ASEStore.cpp

../src/scorched/Release/src3dsparseMeshLODscorched.o : ../src/3dsparse/MeshLOD.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/src3dsparseMeshLODscorched.o ../src/3dsparse/MeshLOD.cpp

../src/scorched/Release/src3dsparseMeshLODTriscorched.o : ../src/3dsparse/MeshLODTri.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/src3dsparseMeshLODTriscorched.o ../src/3dsparse/MeshLODTri.cpp

../src/scorched/Release/src3dsparseMeshLODVectorscorched.o : ../src/3dsparse/MeshLODVector.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/src3dsparseMeshLODVectorscorched.o ../src/3dsparse/MeshLODVector.cpp

../src/scorched/Release/src3dsparseaseFile.lexscorched.o : ../src/3dsparse/aseFile.lex.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/src3dsparseaseFile.lexscorched.o ../src/3dsparse/aseFile.lex.cpp

../src/scorched/Release/src3dsparseaseFile.tabscorched.o : ../src/3dsparse/aseFile.tab.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/src3dsparseaseFile.tabscorched.o ../src/3dsparse/aseFile.tab.cpp

../src/scorched/Release/srccommonARGParserscorched.o : ../src/common/ARGParser.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonARGParserscorched.o ../src/common/ARGParser.cpp

../src/scorched/Release/srccommonDefinesscorched.o : ../src/common/Defines.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonDefinesscorched.o ../src/common/Defines.cpp

../src/scorched/Release/srccommonDisplayscorched.o : ../src/common/Display.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonDisplayscorched.o ../src/common/Display.cpp

../src/scorched/Release/srccommonFacescorched.o : ../src/common/Face.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonFacescorched.o ../src/common/Face.cpp

../src/scorched/Release/srccommonFileLinesscorched.o : ../src/common/FileLines.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonFileLinesscorched.o ../src/common/FileLines.cpp

../src/scorched/Release/srccommonFileListscorched.o : ../src/common/FileList.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonFileListscorched.o ../src/common/FileList.cpp

../src/scorched/Release/srccommonGammascorched.o : ../src/common/Gamma.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonGammascorched.o ../src/common/Gamma.cpp

../src/scorched/Release/srccommonKeyboardscorched.o : ../src/common/Keyboard.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonKeyboardscorched.o ../src/common/Keyboard.cpp

../src/scorched/Release/srccommonLinescorched.o : ../src/common/Line.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonLinescorched.o ../src/common/Line.cpp

../src/scorched/Release/srccommonLoggerscorched.o : ../src/common/Logger.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonLoggerscorched.o ../src/common/Logger.cpp

../src/scorched/Release/srccommonMousescorched.o : ../src/common/Mouse.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonMousescorched.o ../src/common/Mouse.cpp

../src/scorched/Release/srccommonOptionEntryscorched.o : ../src/common/OptionEntry.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonOptionEntryscorched.o ../src/common/OptionEntry.cpp

../src/scorched/Release/srccommonOptionsDisplayscorched.o : ../src/common/OptionsDisplay.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonOptionsDisplayscorched.o ../src/common/OptionsDisplay.cpp

../src/scorched/Release/srccommonOptionsGamescorched.o : ../src/common/OptionsGame.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonOptionsGamescorched.o ../src/common/OptionsGame.cpp

../src/scorched/Release/srccommonOptionsParamscorched.o : ../src/common/OptionsParam.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonOptionsParamscorched.o ../src/common/OptionsParam.cpp

../src/scorched/Release/srccommonOptionsTransientscorched.o : ../src/common/OptionsTransient.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonOptionsTransientscorched.o ../src/common/OptionsTransient.cpp

../src/scorched/Release/srccommonProgressCounterscorched.o : ../src/common/ProgressCounter.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonProgressCounterscorched.o ../src/common/ProgressCounter.cpp

../src/scorched/Release/srccommonRandomGeneratorscorched.o : ../src/common/RandomGenerator.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonRandomGeneratorscorched.o ../src/common/RandomGenerator.cpp

../src/scorched/Release/srccommonRegistryscorched.o : ../src/common/Registry.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonRegistryscorched.o ../src/common/Registry.cpp

../src/scorched/Release/srccommonResourceFilescorched.o : ../src/common/ResourceFile.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonResourceFilescorched.o ../src/common/ResourceFile.cpp

../src/scorched/Release/srccommonResourcesscorched.o : ../src/common/Resources.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonResourcesscorched.o ../src/common/Resources.cpp

../src/scorched/Release/srccommonSoundscorched.o : ../src/common/Sound.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonSoundscorched.o ../src/common/Sound.cpp

../src/scorched/Release/srccommonSoundBufferscorched.o : ../src/common/SoundBuffer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonSoundBufferscorched.o ../src/common/SoundBuffer.cpp

../src/scorched/Release/srccommonSoundStorescorched.o : ../src/common/SoundStore.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonSoundStorescorched.o ../src/common/SoundStore.cpp

../src/scorched/Release/srccommonTimerscorched.o : ../src/common/Timer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonTimerscorched.o ../src/common/Timer.cpp

../src/scorched/Release/srccommonTrianglescorched.o : ../src/common/Triangle.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonTrianglescorched.o ../src/common/Triangle.cpp

../src/scorched/Release/srccommonVectorscorched.o : ../src/common/Vector.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonVectorscorched.o ../src/common/Vector.cpp

../src/scorched/Release/srccommonVectorLibscorched.o : ../src/common/VectorLib.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonVectorLibscorched.o ../src/common/VectorLib.cpp

../src/scorched/Release/srccommonWindowManagerscorched.o : ../src/common/WindowManager.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccommonWindowManagerscorched.o ../src/common/WindowManager.cpp

../src/scorched/Release/srcGLWGLWButtonscorched.o : ../src/GLW/GLWButton.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWButtonscorched.o ../src/GLW/GLWButton.cpp

../src/scorched/Release/srcGLWGLWCheckBoxscorched.o : ../src/GLW/GLWCheckBox.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWCheckBoxscorched.o ../src/GLW/GLWCheckBox.cpp

../src/scorched/Release/srcGLWGLWDropDownscorched.o : ../src/GLW/GLWDropDown.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWDropDownscorched.o ../src/GLW/GLWDropDown.cpp

../src/scorched/Release/srcGLWGLWFileViewscorched.o : ../src/GLW/GLWFileView.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWFileViewscorched.o ../src/GLW/GLWFileView.cpp

../src/scorched/Release/srcGLWGLWFlagscorched.o : ../src/GLW/GLWFlag.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWFlagscorched.o ../src/GLW/GLWFlag.cpp

../src/scorched/Release/srcGLWGLWFontscorched.o : ../src/GLW/GLWFont.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWFontscorched.o ../src/GLW/GLWFont.cpp

../src/scorched/Release/srcGLWGLWLabelscorched.o : ../src/GLW/GLWLabel.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWLabelscorched.o ../src/GLW/GLWLabel.cpp

../src/scorched/Release/srcGLWGLWListViewscorched.o : ../src/GLW/GLWListView.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWListViewscorched.o ../src/GLW/GLWListView.cpp

../src/scorched/Release/srcGLWGLWPanelscorched.o : ../src/GLW/GLWPanel.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWPanelscorched.o ../src/GLW/GLWPanel.cpp

../src/scorched/Release/srcGLWGLWProgressscorched.o : ../src/GLW/GLWProgress.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWProgressscorched.o ../src/GLW/GLWProgress.cpp

../src/scorched/Release/srcGLWGLWPushButtonscorched.o : ../src/GLW/GLWPushButton.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWPushButtonscorched.o ../src/GLW/GLWPushButton.cpp

../src/scorched/Release/srcGLWGLWScrollButtonscorched.o : ../src/GLW/GLWScrollButton.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWScrollButtonscorched.o ../src/GLW/GLWScrollButton.cpp

../src/scorched/Release/srcGLWGLWScrollHscorched.o : ../src/GLW/GLWScrollH.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWScrollHscorched.o ../src/GLW/GLWScrollH.cpp

../src/scorched/Release/srcGLWGLWScrollPanelscorched.o : ../src/GLW/GLWScrollPanel.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWScrollPanelscorched.o ../src/GLW/GLWScrollPanel.cpp

../src/scorched/Release/srcGLWGLWScrollWscorched.o : ../src/GLW/GLWScrollW.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWScrollWscorched.o ../src/GLW/GLWScrollW.cpp

../src/scorched/Release/srcGLWGLWSpinBoxscorched.o : ../src/GLW/GLWSpinBox.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWSpinBoxscorched.o ../src/GLW/GLWSpinBox.cpp

../src/scorched/Release/srcGLWGLWTabscorched.o : ../src/GLW/GLWTab.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWTabscorched.o ../src/GLW/GLWTab.cpp

../src/scorched/Release/srcGLWGLWTextBoxscorched.o : ../src/GLW/GLWTextBox.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWTextBoxscorched.o ../src/GLW/GLWTextBox.cpp

../src/scorched/Release/srcGLWGLWTextButtonscorched.o : ../src/GLW/GLWTextButton.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWTextButtonscorched.o ../src/GLW/GLWTextButton.cpp

../src/scorched/Release/srcGLWGLWVisiblePanelscorched.o : ../src/GLW/GLWVisiblePanel.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWVisiblePanelscorched.o ../src/GLW/GLWVisiblePanel.cpp

../src/scorched/Release/srcGLWGLWVisibleWidgetscorched.o : ../src/GLW/GLWVisibleWidget.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWVisibleWidgetscorched.o ../src/GLW/GLWVisibleWidget.cpp

../src/scorched/Release/srcGLWGLWWindowscorched.o : ../src/GLW/GLWWindow.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWWindowscorched.o ../src/GLW/GLWWindow.cpp

../src/scorched/Release/srcGLWGLWidgetscorched.o : ../src/GLW/GLWidget.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcGLWGLWidgetscorched.o ../src/GLW/GLWidget.cpp

../src/scorched/Release/srcspritesDebrisActionRendererscorched.o : ../src/sprites/DebrisActionRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcspritesDebrisActionRendererscorched.o ../src/sprites/DebrisActionRenderer.cpp

../src/scorched/Release/srcspritesExplosionNukeRendererscorched.o : ../src/sprites/ExplosionNukeRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcspritesExplosionNukeRendererscorched.o ../src/sprites/ExplosionNukeRenderer.cpp

../src/scorched/Release/srcspritesExplosionRendererscorched.o : ../src/sprites/ExplosionRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcspritesExplosionRendererscorched.o ../src/sprites/ExplosionRenderer.cpp

../src/scorched/Release/srcspritesExplosionTexturesscorched.o : ../src/sprites/ExplosionTextures.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcspritesExplosionTexturesscorched.o ../src/sprites/ExplosionTextures.cpp

../src/scorched/Release/srcspritesFlareActionRendererscorched.o : ../src/sprites/FlareActionRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcspritesFlareActionRendererscorched.o ../src/sprites/FlareActionRenderer.cpp

../src/scorched/Release/srcspritesSmokeActionRendererscorched.o : ../src/sprites/SmokeActionRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcspritesSmokeActionRendererscorched.o ../src/sprites/SmokeActionRenderer.cpp

../src/scorched/Release/srcspritesTalkRendererscorched.o : ../src/sprites/TalkRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcspritesTalkRendererscorched.o ../src/sprites/TalkRenderer.cpp

../src/scorched/Release/srcspritesTankDeadRendererscorched.o : ../src/sprites/TankDeadRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcspritesTankDeadRendererscorched.o ../src/sprites/TankDeadRenderer.cpp

../src/scorched/Release/srcspritesTextActionRendererscorched.o : ../src/sprites/TextActionRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcspritesTextActionRendererscorched.o ../src/sprites/TextActionRenderer.cpp

../src/scorched/Release/srcspritesTracerActionRendererscorched.o : ../src/sprites/TracerActionRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcspritesTracerActionRendererscorched.o ../src/sprites/TracerActionRenderer.cpp

../src/scorched/Release/srcspritesWallActionRendererscorched.o : ../src/sprites/WallActionRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcspritesWallActionRendererscorched.o ../src/sprites/WallActionRenderer.cpp

../src/scorched/Release/srclandscapeCloudSimscorched.o : ../src/landscape/CloudSim.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeCloudSimscorched.o ../src/landscape/CloudSim.cpp

../src/scorched/Release/srclandscapeDeformLandscapescorched.o : ../src/landscape/DeformLandscape.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeDeformLandscapescorched.o ../src/landscape/DeformLandscape.cpp

../src/scorched/Release/srclandscapeDeformTexturesscorched.o : ../src/landscape/DeformTextures.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeDeformTexturesscorched.o ../src/landscape/DeformTextures.cpp

../src/scorched/Release/srclandscapeGlobalHMapscorched.o : ../src/landscape/GlobalHMap.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeGlobalHMapscorched.o ../src/landscape/GlobalHMap.cpp

../src/scorched/Release/srclandscapeHeightMapscorched.o : ../src/landscape/HeightMap.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeHeightMapscorched.o ../src/landscape/HeightMap.cpp

../src/scorched/Release/srclandscapeHeightMapCollisionscorched.o : ../src/landscape/HeightMapCollision.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeHeightMapCollisionscorched.o ../src/landscape/HeightMapCollision.cpp

../src/scorched/Release/srclandscapeHeightMapLoaderscorched.o : ../src/landscape/HeightMapLoader.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeHeightMapLoaderscorched.o ../src/landscape/HeightMapLoader.cpp

../src/scorched/Release/srclandscapeHeightMapModifierscorched.o : ../src/landscape/HeightMapModifier.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeHeightMapModifierscorched.o ../src/landscape/HeightMapModifier.cpp

../src/scorched/Release/srclandscapeHeightMapRendererscorched.o : ../src/landscape/HeightMapRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeHeightMapRendererscorched.o ../src/landscape/HeightMapRenderer.cpp

../src/scorched/Release/srclandscapeHeightMapSurroundscorched.o : ../src/landscape/HeightMapSurround.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeHeightMapSurroundscorched.o ../src/landscape/HeightMapSurround.cpp

../src/scorched/Release/srclandscapeHemispherescorched.o : ../src/landscape/Hemisphere.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeHemispherescorched.o ../src/landscape/Hemisphere.cpp

../src/scorched/Release/srclandscapeInfoMapscorched.o : ../src/landscape/InfoMap.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeInfoMapscorched.o ../src/landscape/InfoMap.cpp

../src/scorched/Release/srclandscapeLanscapescorched.o : ../src/landscape/Lanscape.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeLanscapescorched.o ../src/landscape/Lanscape.cpp

../src/scorched/Release/srclandscapeMovementMapscorched.o : ../src/landscape/MovementMap.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeMovementMapscorched.o ../src/landscape/MovementMap.cpp

../src/scorched/Release/srclandscapeNapalmMapscorched.o : ../src/landscape/NapalmMap.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeNapalmMapscorched.o ../src/landscape/NapalmMap.cpp

../src/scorched/Release/srclandscapePatchscorched.o : ../src/landscape/Patch.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapePatchscorched.o ../src/landscape/Patch.cpp

../src/scorched/Release/srclandscapePatchGridscorched.o : ../src/landscape/PatchGrid.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapePatchGridscorched.o ../src/landscape/PatchGrid.cpp

../src/scorched/Release/srclandscapePatchSidescorched.o : ../src/landscape/PatchSide.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapePatchSidescorched.o ../src/landscape/PatchSide.cpp

../src/scorched/Release/srclandscapePatchVarscorched.o : ../src/landscape/PatchVar.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapePatchVarscorched.o ../src/landscape/PatchVar.cpp

../src/scorched/Release/srclandscapeShadowMapscorched.o : ../src/landscape/ShadowMap.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeShadowMapscorched.o ../src/landscape/ShadowMap.cpp

../src/scorched/Release/srclandscapeSmokescorched.o : ../src/landscape/Smoke.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeSmokescorched.o ../src/landscape/Smoke.cpp

../src/scorched/Release/srclandscapeSmokeChainscorched.o : ../src/landscape/SmokeChain.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeSmokeChainscorched.o ../src/landscape/SmokeChain.cpp

../src/scorched/Release/srclandscapeSunscorched.o : ../src/landscape/Sun.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeSunscorched.o ../src/landscape/Sun.cpp

../src/scorched/Release/srclandscapeSurroundscorched.o : ../src/landscape/Surround.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeSurroundscorched.o ../src/landscape/Surround.cpp

../src/scorched/Release/srclandscapeSurroundDefsscorched.o : ../src/landscape/SurroundDefs.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeSurroundDefsscorched.o ../src/landscape/SurroundDefs.cpp

../src/scorched/Release/srclandscapeTriNodescorched.o : ../src/landscape/TriNode.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeTriNodescorched.o ../src/landscape/TriNode.cpp

../src/scorched/Release/srclandscapeTriNodePoolscorched.o : ../src/landscape/TriNodePool.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeTriNodePoolscorched.o ../src/landscape/TriNodePool.cpp

../src/scorched/Release/srclandscapeVariancescorched.o : ../src/landscape/Variance.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeVariancescorched.o ../src/landscape/Variance.cpp

../src/scorched/Release/srclandscapeWallscorched.o : ../src/landscape/Wall.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeWallscorched.o ../src/landscape/Wall.cpp

../src/scorched/Release/srclandscapeWaterMapscorched.o : ../src/landscape/WaterMap.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeWaterMapscorched.o ../src/landscape/WaterMap.cpp

../src/scorched/Release/srclandscapeWaterMapModifierscorched.o : ../src/landscape/WaterMapModifier.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeWaterMapModifierscorched.o ../src/landscape/WaterMapModifier.cpp

../src/scorched/Release/srclandscapeWaterMapPointsscorched.o : ../src/landscape/WaterMapPoints.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeWaterMapPointsscorched.o ../src/landscape/WaterMapPoints.cpp

../src/scorched/Release/srclandscapeWaterMapSurroundscorched.o : ../src/landscape/WaterMapSurround.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srclandscapeWaterMapSurroundscorched.o ../src/landscape/WaterMapSurround.cpp

../src/scorched/Release/srcscorchedCreateDialogsscorched.o : ../src/scorched//CreateDialogs.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcscorchedCreateDialogsscorched.o ../src/scorched//CreateDialogs.cpp

../src/scorched/Release/srcscorchedDisplayDialogscorched.o : ../src/scorched//DisplayDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcscorchedDisplayDialogscorched.o ../src/scorched//DisplayDialog.cpp

../src/scorched/Release/srcscorchedMainDialogscorched.o : ../src/scorched//MainDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcscorchedMainDialogscorched.o ../src/scorched//MainDialog.cpp

../src/scorched/Release/srcscorchedNetDialogscorched.o : ../src/scorched//NetDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcscorchedNetDialogscorched.o ../src/scorched//NetDialog.cpp

../src/scorched/Release/srcscorchedServerDialogscorched.o : ../src/scorched//ServerDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcscorchedServerDialogscorched.o ../src/scorched//ServerDialog.cpp

../src/scorched/Release/srcscorchedServerSDialogscorched.o : ../src/scorched//ServerSDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcscorchedServerSDialogscorched.o ../src/scorched//ServerSDialog.cpp

../src/scorched/Release/srcscorchedSettingsDialogscorched.o : ../src/scorched//SettingsDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcscorchedSettingsDialogscorched.o ../src/scorched//SettingsDialog.cpp

../src/scorched/Release/srcscorchedSingleDialogscorched.o : ../src/scorched//SingleDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcscorchedSingleDialogscorched.o ../src/scorched//SingleDialog.cpp

../src/scorched/Release/srcscorchedmainscorched.o : ../src/scorched//main.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcscorchedmainscorched.o ../src/scorched//main.cpp

../src/scorched/Release/srctankTankscorched.o : ../src/tank/Tank.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankscorched.o ../src/tank/Tank.cpp

../src/scorched/Release/srctankTankAIscorched.o : ../src/tank/TankAI.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankAIscorched.o ../src/tank/TankAI.cpp

../src/scorched/Release/srctankTankAILogicscorched.o : ../src/tank/TankAILogic.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankAILogicscorched.o ../src/tank/TankAILogic.cpp

../src/scorched/Release/srctankTankAccessoriesscorched.o : ../src/tank/TankAccessories.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankAccessoriesscorched.o ../src/tank/TankAccessories.cpp

../src/scorched/Release/srctankTankAutoDefensescorched.o : ../src/tank/TankAutoDefense.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankAutoDefensescorched.o ../src/tank/TankAutoDefense.cpp

../src/scorched/Release/srctankTankBatteriesscorched.o : ../src/tank/TankBatteries.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankBatteriesscorched.o ../src/tank/TankBatteries.cpp

../src/scorched/Release/srctankTankColorGeneratorscorched.o : ../src/tank/TankColorGenerator.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankColorGeneratorscorched.o ../src/tank/TankColorGenerator.cpp

../src/scorched/Release/srctankTankContainerscorched.o : ../src/tank/TankContainer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankContainerscorched.o ../src/tank/TankContainer.cpp

../src/scorched/Release/srctankTankControllerscorched.o : ../src/tank/TankController.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankControllerscorched.o ../src/tank/TankController.cpp

../src/scorched/Release/srctankTankFuelscorched.o : ../src/tank/TankFuel.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankFuelscorched.o ../src/tank/TankFuel.cpp

../src/scorched/Release/srctankTankLibscorched.o : ../src/tank/TankLib.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankLibscorched.o ../src/tank/TankLib.cpp

../src/scorched/Release/srctankTankModelIdscorched.o : ../src/tank/TankModelId.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankModelIdscorched.o ../src/tank/TankModelId.cpp

../src/scorched/Release/srctankTankParachutesscorched.o : ../src/tank/TankParachutes.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankParachutesscorched.o ../src/tank/TankParachutes.cpp

../src/scorched/Release/srctankTankPhysicsscorched.o : ../src/tank/TankPhysics.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankPhysicsscorched.o ../src/tank/TankPhysics.cpp

../src/scorched/Release/srctankTankScorescorched.o : ../src/tank/TankScore.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankScorescorched.o ../src/tank/TankScore.cpp

../src/scorched/Release/srctankTankShieldsscorched.o : ../src/tank/TankShields.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankShieldsscorched.o ../src/tank/TankShields.cpp

../src/scorched/Release/srctankTankSortscorched.o : ../src/tank/TankSort.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankSortscorched.o ../src/tank/TankSort.cpp

../src/scorched/Release/srctankTankStartscorched.o : ../src/tank/TankStart.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankStartscorched.o ../src/tank/TankStart.cpp

../src/scorched/Release/srctankTankStatescorched.o : ../src/tank/TankState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankStatescorched.o ../src/tank/TankState.cpp

../src/scorched/Release/srctankTankWeaponscorched.o : ../src/tank/TankWeapon.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankTankWeaponscorched.o ../src/tank/TankWeapon.cpp

../src/scorched/Release/srcXMLXMLFilescorched.o : ../src/XML/XMLFile.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcXMLXMLFilescorched.o ../src/XML/XMLFile.cpp

../src/scorched/Release/srcXMLXMLParserscorched.o : ../src/XML/XMLParser.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcXMLXMLParserscorched.o ../src/XML/XMLParser.cpp

../src/scorched/Release/srcXMLxmlparsescorched.o : ../src/XML/xmlparse.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcXMLxmlparsescorched.o ../src/XML/xmlparse.c

../src/scorched/Release/srcXMLxmlrolescorched.o : ../src/XML/xmlrole.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcXMLxmlrolescorched.o ../src/XML/xmlrole.c

../src/scorched/Release/srcXMLxmltokscorched.o : ../src/XML/xmltok.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcXMLxmltokscorched.o ../src/XML/xmltok.c

../src/scorched/Release/srczlibadler32scorched.o : ../src/zlib/adler32.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srczlibadler32scorched.o ../src/zlib/adler32.c

../src/scorched/Release/srczlibcompressscorched.o : ../src/zlib/compress.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srczlibcompressscorched.o ../src/zlib/compress.c

../src/scorched/Release/srczlibcrc32scorched.o : ../src/zlib/crc32.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srczlibcrc32scorched.o ../src/zlib/crc32.c

../src/scorched/Release/srczlibdeflatescorched.o : ../src/zlib/deflate.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srczlibdeflatescorched.o ../src/zlib/deflate.c

../src/scorched/Release/srczlibgzioscorched.o : ../src/zlib/gzio.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srczlibgzioscorched.o ../src/zlib/gzio.c

../src/scorched/Release/srczlibinfblockscorched.o : ../src/zlib/infblock.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srczlibinfblockscorched.o ../src/zlib/infblock.c

../src/scorched/Release/srczlibinfcodesscorched.o : ../src/zlib/infcodes.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srczlibinfcodesscorched.o ../src/zlib/infcodes.c

../src/scorched/Release/srczlibinffastscorched.o : ../src/zlib/inffast.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srczlibinffastscorched.o ../src/zlib/inffast.c

../src/scorched/Release/srczlibinflatescorched.o : ../src/zlib/inflate.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srczlibinflatescorched.o ../src/zlib/inflate.c

../src/scorched/Release/srczlibinftreesscorched.o : ../src/zlib/inftrees.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srczlibinftreesscorched.o ../src/zlib/inftrees.c

../src/scorched/Release/srczlibinfutilscorched.o : ../src/zlib/infutil.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srczlibinfutilscorched.o ../src/zlib/infutil.c

../src/scorched/Release/srczlibtreesscorched.o : ../src/zlib/trees.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srczlibtreesscorched.o ../src/zlib/trees.c

../src/scorched/Release/srczlibuncomprscorched.o : ../src/zlib/uncompr.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srczlibuncomprscorched.o ../src/zlib/uncompr.c

../src/scorched/Release/srczlibzutilscorched.o : ../src/zlib/zutil.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srczlibzutilscorched.o ../src/zlib/zutil.c

../src/scorched/Release/srcactionsExplosionscorched.o : ../src/actions/Explosion.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsExplosionscorched.o ../src/actions/Explosion.cpp

../src/scorched/Release/srcactionsNapalmscorched.o : ../src/actions/Napalm.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsNapalmscorched.o ../src/actions/Napalm.cpp

../src/scorched/Release/srcactionsShieldHitscorched.o : ../src/actions/ShieldHit.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsShieldHitscorched.o ../src/actions/ShieldHit.cpp

../src/scorched/Release/srcactionsShotProjectilescorched.o : ../src/actions/ShotProjectile.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsShotProjectilescorched.o ../src/actions/ShotProjectile.cpp

../src/scorched/Release/srcactionsShotProjectileClodscorched.o : ../src/actions/ShotProjectileClod.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsShotProjectileClodscorched.o ../src/actions/ShotProjectileClod.cpp

../src/scorched/Release/srcactionsShotProjectileExplosionscorched.o : ../src/actions/ShotProjectileExplosion.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsShotProjectileExplosionscorched.o ../src/actions/ShotProjectileExplosion.cpp

../src/scorched/Release/srcactionsShotProjectileFunkyscorched.o : ../src/actions/ShotProjectileFunky.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsShotProjectileFunkyscorched.o ../src/actions/ShotProjectileFunky.cpp

../src/scorched/Release/srcactionsShotProjectileMirvscorched.o : ../src/actions/ShotProjectileMirv.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsShotProjectileMirvscorched.o ../src/actions/ShotProjectileMirv.cpp

../src/scorched/Release/srcactionsShotProjectileNapalmscorched.o : ../src/actions/ShotProjectileNapalm.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsShotProjectileNapalmscorched.o ../src/actions/ShotProjectileNapalm.cpp

../src/scorched/Release/srcactionsShotProjectileRiotscorched.o : ../src/actions/ShotProjectileRiot.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsShotProjectileRiotscorched.o ../src/actions/ShotProjectileRiot.cpp

../src/scorched/Release/srcactionsShotProjectileTracerscorched.o : ../src/actions/ShotProjectileTracer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsShotProjectileTracerscorched.o ../src/actions/ShotProjectileTracer.cpp

../src/scorched/Release/srcactionsShowScorescorched.o : ../src/actions/ShowScore.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsShowScorescorched.o ../src/actions/ShowScore.cpp

../src/scorched/Release/srcactionsSpriteProjectilescorched.o : ../src/actions/SpriteProjectile.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsSpriteProjectilescorched.o ../src/actions/SpriteProjectile.cpp

../src/scorched/Release/srcactionsTankDamagescorched.o : ../src/actions/TankDamage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsTankDamagescorched.o ../src/actions/TankDamage.cpp

../src/scorched/Release/srcactionsTankDeadscorched.o : ../src/actions/TankDead.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsTankDeadscorched.o ../src/actions/TankDead.cpp

../src/scorched/Release/srcactionsTankFallingscorched.o : ../src/actions/TankFalling.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsTankFallingscorched.o ../src/actions/TankFalling.cpp

../src/scorched/Release/srcactionsTankFiredscorched.o : ../src/actions/TankFired.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsTankFiredscorched.o ../src/actions/TankFired.cpp

../src/scorched/Release/srcactionsTankMovescorched.o : ../src/actions/TankMove.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsTankMovescorched.o ../src/actions/TankMove.cpp

../src/scorched/Release/srcactionsTankMovementscorched.o : ../src/actions/TankMovement.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsTankMovementscorched.o ../src/actions/TankMovement.cpp

../src/scorched/Release/srcactionsTankResignscorched.o : ../src/actions/TankResign.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsTankResignscorched.o ../src/actions/TankResign.cpp

../src/scorched/Release/srcactionsTankSayscorched.o : ../src/actions/TankSay.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsTankSayscorched.o ../src/actions/TankSay.cpp

../src/scorched/Release/srcactionsTankScoredscorched.o : ../src/actions/TankScored.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsTankScoredscorched.o ../src/actions/TankScored.cpp

../src/scorched/Release/srcactionsWallHitscorched.o : ../src/actions/WallHit.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcactionsWallHitscorched.o ../src/actions/WallHit.cpp

../src/scorched/Release/srccomsComsActionsMessagescorched.o : ../src/coms/ComsActionsMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsActionsMessagescorched.o ../src/coms/ComsActionsMessage.cpp

../src/scorched/Release/srccomsComsAddPlayerMessagescorched.o : ../src/coms/ComsAddPlayerMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsAddPlayerMessagescorched.o ../src/coms/ComsAddPlayerMessage.cpp

../src/scorched/Release/srccomsComsBuyAccessoryMessagescorched.o : ../src/coms/ComsBuyAccessoryMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsBuyAccessoryMessagescorched.o ../src/coms/ComsBuyAccessoryMessage.cpp

../src/scorched/Release/srccomsComsConnectAcceptMessagescorched.o : ../src/coms/ComsConnectAcceptMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsConnectAcceptMessagescorched.o ../src/coms/ComsConnectAcceptMessage.cpp

../src/scorched/Release/srccomsComsConnectMessagescorched.o : ../src/coms/ComsConnectMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsConnectMessagescorched.o ../src/coms/ComsConnectMessage.cpp

../src/scorched/Release/srccomsComsDefenseMessagescorched.o : ../src/coms/ComsDefenseMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsDefenseMessagescorched.o ../src/coms/ComsDefenseMessage.cpp

../src/scorched/Release/srccomsComsGameStoppedMessagescorched.o : ../src/coms/ComsGameStoppedMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsGameStoppedMessagescorched.o ../src/coms/ComsGameStoppedMessage.cpp

../src/scorched/Release/srccomsComsLevelMessagescorched.o : ../src/coms/ComsLevelMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsLevelMessagescorched.o ../src/coms/ComsLevelMessage.cpp

../src/scorched/Release/srccomsComsMessagescorched.o : ../src/coms/ComsMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsMessagescorched.o ../src/coms/ComsMessage.cpp

../src/scorched/Release/srccomsComsMessageHandlerscorched.o : ../src/coms/ComsMessageHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsMessageHandlerscorched.o ../src/coms/ComsMessageHandler.cpp

../src/scorched/Release/srccomsComsMessageSenderscorched.o : ../src/coms/ComsMessageSender.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsMessageSenderscorched.o ../src/coms/ComsMessageSender.cpp

../src/scorched/Release/srccomsComsNewGameMessagescorched.o : ../src/coms/ComsNewGameMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsNewGameMessagescorched.o ../src/coms/ComsNewGameMessage.cpp

../src/scorched/Release/srccomsComsPlayedMoveMessagescorched.o : ../src/coms/ComsPlayedMoveMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsPlayedMoveMessagescorched.o ../src/coms/ComsPlayedMoveMessage.cpp

../src/scorched/Release/srccomsComsPlayerReadyMessagescorched.o : ../src/coms/ComsPlayerReadyMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsPlayerReadyMessagescorched.o ../src/coms/ComsPlayerReadyMessage.cpp

../src/scorched/Release/srccomsComsPlayerStateMessagescorched.o : ../src/coms/ComsPlayerStateMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsPlayerStateMessagescorched.o ../src/coms/ComsPlayerStateMessage.cpp

../src/scorched/Release/srccomsComsRmPlayerMessagescorched.o : ../src/coms/ComsRmPlayerMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsRmPlayerMessagescorched.o ../src/coms/ComsRmPlayerMessage.cpp

../src/scorched/Release/srccomsComsScoreMessagescorched.o : ../src/coms/ComsScoreMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsScoreMessagescorched.o ../src/coms/ComsScoreMessage.cpp

../src/scorched/Release/srccomsComsStartGameMessagescorched.o : ../src/coms/ComsStartGameMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsStartGameMessagescorched.o ../src/coms/ComsStartGameMessage.cpp

../src/scorched/Release/srccomsComsTextMessagescorched.o : ../src/coms/ComsTextMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsComsTextMessagescorched.o ../src/coms/ComsTextMessage.cpp

../src/scorched/Release/srccomsNetBufferscorched.o : ../src/coms/NetBuffer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsNetBufferscorched.o ../src/coms/NetBuffer.cpp

../src/scorched/Release/srccomsNetBufferUtilscorched.o : ../src/coms/NetBufferUtil.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsNetBufferUtilscorched.o ../src/coms/NetBufferUtil.cpp

../src/scorched/Release/srccomsNetClientscorched.o : ../src/coms/NetClient.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsNetClientscorched.o ../src/coms/NetClient.cpp

../src/scorched/Release/srccomsNetMessagescorched.o : ../src/coms/NetMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsNetMessagescorched.o ../src/coms/NetMessage.cpp

../src/scorched/Release/srccomsNetMessageHandlerscorched.o : ../src/coms/NetMessageHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsNetMessageHandlerscorched.o ../src/coms/NetMessageHandler.cpp

../src/scorched/Release/srccomsNetMessagePoolscorched.o : ../src/coms/NetMessagePool.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsNetMessagePoolscorched.o ../src/coms/NetMessagePool.cpp

../src/scorched/Release/srccomsNetServerscorched.o : ../src/coms/NetServer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srccomsNetServerscorched.o ../src/coms/NetServer.cpp

../src/scorched/Release/srcengineActionscorched.o : ../src/engine/Action.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcengineActionscorched.o ../src/engine/Action.cpp

../src/scorched/Release/srcengineActionBufferscorched.o : ../src/engine/ActionBuffer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcengineActionBufferscorched.o ../src/engine/ActionBuffer.cpp

../src/scorched/Release/srcengineActionControllerscorched.o : ../src/engine/ActionController.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcengineActionControllerscorched.o ../src/engine/ActionController.cpp

../src/scorched/Release/srcengineActionMetascorched.o : ../src/engine/ActionMeta.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcengineActionMetascorched.o ../src/engine/ActionMeta.cpp

../src/scorched/Release/srcengineFrameTimerscorched.o : ../src/engine/FrameTimer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcengineFrameTimerscorched.o ../src/engine/FrameTimer.cpp

../src/scorched/Release/srcengineGameStatescorched.o : ../src/engine/GameState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcengineGameStatescorched.o ../src/engine/GameState.cpp

../src/scorched/Release/srcengineGameStateIscorched.o : ../src/engine/GameStateI.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcengineGameStateIscorched.o ../src/engine/GameStateI.cpp

../src/scorched/Release/srcengineGameStateStimulusIscorched.o : ../src/engine/GameStateStimulusI.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcengineGameStateStimulusIscorched.o ../src/engine/GameStateStimulusI.cpp

../src/scorched/Release/srcengineMainLoopscorched.o : ../src/engine/MainLoop.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcengineMainLoopscorched.o ../src/engine/MainLoop.cpp

../src/scorched/Release/srcengineMainLoopIscorched.o : ../src/engine/MainLoopI.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcengineMainLoopIscorched.o ../src/engine/MainLoopI.cpp

../src/scorched/Release/srcenginePhysicsEnginescorched.o : ../src/engine/PhysicsEngine.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcenginePhysicsEnginescorched.o ../src/engine/PhysicsEngine.cpp

../src/scorched/Release/srcenginePhysicsParticlescorched.o : ../src/engine/PhysicsParticle.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcenginePhysicsParticlescorched.o ../src/engine/PhysicsParticle.cpp

../src/scorched/Release/srcenginePhysicsParticleObjectscorched.o : ../src/engine/PhysicsParticleObject.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcenginePhysicsParticleObjectscorched.o ../src/engine/PhysicsParticleObject.cpp

../src/scorched/Release/srcengineScorchedCollisionHandlerscorched.o : ../src/engine/ScorchedCollisionHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcengineScorchedCollisionHandlerscorched.o ../src/engine/ScorchedCollisionHandler.cpp

../src/scorched/Release/srcengineScorchedCollisionIdsscorched.o : ../src/engine/ScorchedCollisionIds.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcengineScorchedCollisionIdsscorched.o ../src/engine/ScorchedCollisionIds.cpp

../src/scorched/Release/srcserverScorchedASEscorched.o : ../src/server/ScorchedASE.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverScorchedASEscorched.o ../src/server/ScorchedASE.cpp

../src/scorched/Release/srcserverServerBuyAccessoryHandlerscorched.o : ../src/server/ServerBuyAccessoryHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerBuyAccessoryHandlerscorched.o ../src/server/ServerBuyAccessoryHandler.cpp

../src/scorched/Release/srcserverServerConnectHandlerscorched.o : ../src/server/ServerConnectHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerConnectHandlerscorched.o ../src/server/ServerConnectHandler.cpp

../src/scorched/Release/srcserverServerDefenseHandlerscorched.o : ../src/server/ServerDefenseHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerDefenseHandlerscorched.o ../src/server/ServerDefenseHandler.cpp

../src/scorched/Release/srcserverServerMainscorched.o : ../src/server/ServerMain.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerMainscorched.o ../src/server/ServerMain.cpp

../src/scorched/Release/srcserverServerMessageHandlerscorched.o : ../src/server/ServerMessageHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerMessageHandlerscorched.o ../src/server/ServerMessageHandler.cpp

../src/scorched/Release/srcserverServerNewGameStatescorched.o : ../src/server/ServerNewGameState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerNewGameStatescorched.o ../src/server/ServerNewGameState.cpp

../src/scorched/Release/srcserverServerNextRoundStatescorched.o : ../src/server/ServerNextRoundState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerNextRoundStatescorched.o ../src/server/ServerNextRoundState.cpp

../src/scorched/Release/srcserverServerPlayedMoveHandlerscorched.o : ../src/server/ServerPlayedMoveHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerPlayedMoveHandlerscorched.o ../src/server/ServerPlayedMoveHandler.cpp

../src/scorched/Release/srcserverServerPlayerReadyHandlerscorched.o : ../src/server/ServerPlayerReadyHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerPlayerReadyHandlerscorched.o ../src/server/ServerPlayerReadyHandler.cpp

../src/scorched/Release/srcserverServerPlayingStatescorched.o : ../src/server/ServerPlayingState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerPlayingStatescorched.o ../src/server/ServerPlayingState.cpp

../src/scorched/Release/srcserverServerScoreStatescorched.o : ../src/server/ServerScoreState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerScoreStatescorched.o ../src/server/ServerScoreState.cpp

../src/scorched/Release/srcserverServerShotHolderscorched.o : ../src/server/ServerShotHolder.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerShotHolderscorched.o ../src/server/ServerShotHolder.cpp

../src/scorched/Release/srcserverServerShotStatescorched.o : ../src/server/ServerShotState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerShotStatescorched.o ../src/server/ServerShotState.cpp

../src/scorched/Release/srcserverServerStatescorched.o : ../src/server/ServerState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerStatescorched.o ../src/server/ServerState.cpp

../src/scorched/Release/srcserverServerTextHandlerscorched.o : ../src/server/ServerTextHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerTextHandlerscorched.o ../src/server/ServerTextHandler.cpp

../src/scorched/Release/srcserverServerTooFewPlayersStimulusscorched.o : ../src/server/ServerTooFewPlayersStimulus.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerTooFewPlayersStimulusscorched.o ../src/server/ServerTooFewPlayersStimulus.cpp

../src/scorched/Release/srcserverServerWaitingForPlayersStatescorched.o : ../src/server/ServerWaitingForPlayersState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcserverServerWaitingForPlayersStatescorched.o ../src/server/ServerWaitingForPlayersState.cpp

../src/scorched/Release/srctankaiTankAIAdderscorched.o : ../src/tankai/TankAIAdder.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankaiTankAIAdderscorched.o ../src/tankai/TankAIAdder.cpp

../src/scorched/Release/srctankaiTankAIComputerscorched.o : ../src/tankai/TankAIComputer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankaiTankAIComputerscorched.o ../src/tankai/TankAIComputer.cpp

../src/scorched/Release/srctankaiTankAIComputerBuyerscorched.o : ../src/tankai/TankAIComputerBuyer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankaiTankAIComputerBuyerscorched.o ../src/tankai/TankAIComputerBuyer.cpp

../src/scorched/Release/srctankaiTankAIComputerChooserscorched.o : ../src/tankai/TankAIComputerChooser.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankaiTankAIComputerChooserscorched.o ../src/tankai/TankAIComputerChooser.cpp

../src/scorched/Release/srctankaiTankAIComputerCtrlscorched.o : ../src/tankai/TankAIComputerCtrl.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankaiTankAIComputerCtrlscorched.o ../src/tankai/TankAIComputerCtrl.cpp

../src/scorched/Release/srctankaiTankAIComputerDHTestscorched.o : ../src/tankai/TankAIComputerDHTest.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankaiTankAIComputerDHTestscorched.o ../src/tankai/TankAIComputerDHTest.cpp

../src/scorched/Release/srctankaiTankAIComputerMoronscorched.o : ../src/tankai/TankAIComputerMoron.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankaiTankAIComputerMoronscorched.o ../src/tankai/TankAIComputerMoron.cpp

../src/scorched/Release/srctankaiTankAIComputerPSharkscorched.o : ../src/tankai/TankAIComputerPShark.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankaiTankAIComputerPSharkscorched.o ../src/tankai/TankAIComputerPShark.cpp

../src/scorched/Release/srctankaiTankAIComputerRandomscorched.o : ../src/tankai/TankAIComputerRandom.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankaiTankAIComputerRandomscorched.o ../src/tankai/TankAIComputerRandom.cpp

../src/scorched/Release/srctankaiTankAIComputerShooterscorched.o : ../src/tankai/TankAIComputerShooter.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankaiTankAIComputerShooterscorched.o ../src/tankai/TankAIComputerShooter.cpp

../src/scorched/Release/srctankaiTankAIComputerTosserscorched.o : ../src/tankai/TankAIComputerTosser.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankaiTankAIComputerTosserscorched.o ../src/tankai/TankAIComputerTosser.cpp

../src/scorched/Release/srctankaiTankAIHumanscorched.o : ../src/tankai/TankAIHuman.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankaiTankAIHumanscorched.o ../src/tankai/TankAIHuman.cpp

../src/scorched/Release/srctankaiTankAIHumanCtrlscorched.o : ../src/tankai/TankAIHumanCtrl.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankaiTankAIHumanCtrlscorched.o ../src/tankai/TankAIHumanCtrl.cpp

../src/scorched/Release/srctankaiTankAIStorescorched.o : ../src/tankai/TankAIStore.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankaiTankAIStorescorched.o ../src/tankai/TankAIStore.cpp

../src/scorched/Release/srctankaiTankAIStringsscorched.o : ../src/tankai/TankAIStrings.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srctankaiTankAIStringsscorched.o ../src/tankai/TankAIStrings.cpp

../src/scorched/Release/srcodearrayscorched.o : ../src/ode/array.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodearrayscorched.o ../src/ode/array.cpp

../src/scorched/Release/srcodeerrorscorched.o : ../src/ode/error.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodeerrorscorched.o ../src/ode/error.cpp

../src/scorched/Release/srcodefastdotscorched.o : ../src/ode/fastdot.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodefastdotscorched.o ../src/ode/fastdot.c

../src/scorched/Release/srcodefastldltscorched.o : ../src/ode/fastldlt.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodefastldltscorched.o ../src/ode/fastldlt.c

../src/scorched/Release/srcodefastlsolvescorched.o : ../src/ode/fastlsolve.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodefastlsolvescorched.o ../src/ode/fastlsolve.c

../src/scorched/Release/srcodefastltsolvescorched.o : ../src/ode/fastltsolve.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodefastltsolvescorched.o ../src/ode/fastltsolve.c

../src/scorched/Release/srcodegeomscorched.o : ../src/ode/geom.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodegeomscorched.o ../src/ode/geom.cpp

../src/scorched/Release/srcodejointscorched.o : ../src/ode/joint.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodejointscorched.o ../src/ode/joint.cpp

../src/scorched/Release/srcodelcpscorched.o : ../src/ode/lcp.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodelcpscorched.o ../src/ode/lcp.cpp

../src/scorched/Release/srcodemassscorched.o : ../src/ode/mass.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodemassscorched.o ../src/ode/mass.cpp

../src/scorched/Release/srcodematscorched.o : ../src/ode/mat.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodematscorched.o ../src/ode/mat.cpp

../src/scorched/Release/srcodematrixscorched.o : ../src/ode/matrix.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodematrixscorched.o ../src/ode/matrix.cpp

../src/scorched/Release/srcodememoryscorched.o : ../src/ode/memory.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodememoryscorched.o ../src/ode/memory.cpp

../src/scorched/Release/srcodemiscscorched.o : ../src/ode/misc.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodemiscscorched.o ../src/ode/misc.cpp

../src/scorched/Release/srcodeobstackscorched.o : ../src/ode/obstack.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodeobstackscorched.o ../src/ode/obstack.cpp

../src/scorched/Release/srcodeodescorched.o : ../src/ode/ode.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodeodescorched.o ../src/ode/ode.cpp

../src/scorched/Release/srcodeodemathscorched.o : ../src/ode/odemath.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodeodemathscorched.o ../src/ode/odemath.cpp

../src/scorched/Release/srcoderotationscorched.o : ../src/ode/rotation.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcoderotationscorched.o ../src/ode/rotation.cpp

../src/scorched/Release/srcodespacescorched.o : ../src/ode/space.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodespacescorched.o ../src/ode/space.cpp

../src/scorched/Release/srcodestepscorched.o : ../src/ode/step.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodestepscorched.o ../src/ode/step.cpp

../src/scorched/Release/srcodetestingscorched.o : ../src/ode/testing.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcodetestingscorched.o ../src/ode/testing.cpp

../src/scorched/Release/srcweaponsAccessoryscorched.o : ../src/weapons/Accessory.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsAccessoryscorched.o ../src/weapons/Accessory.cpp

../src/scorched/Release/srcweaponsAccessoryStorescorched.o : ../src/weapons/AccessoryStore.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsAccessoryStorescorched.o ../src/weapons/AccessoryStore.cpp

../src/scorched/Release/srcweaponsAutoDefensescorched.o : ../src/weapons/AutoDefense.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsAutoDefensescorched.o ../src/weapons/AutoDefense.cpp

../src/scorched/Release/srcweaponsBatteryscorched.o : ../src/weapons/Battery.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsBatteryscorched.o ../src/weapons/Battery.cpp

../src/scorched/Release/srcweaponsFuelscorched.o : ../src/weapons/Fuel.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsFuelscorched.o ../src/weapons/Fuel.cpp

../src/scorched/Release/srcweaponsParachutescorched.o : ../src/weapons/Parachute.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsParachutescorched.o ../src/weapons/Parachute.cpp

../src/scorched/Release/srcweaponsShieldscorched.o : ../src/weapons/Shield.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsShieldscorched.o ../src/weapons/Shield.cpp

../src/scorched/Release/srcweaponsShieldReflectivescorched.o : ../src/weapons/ShieldReflective.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsShieldReflectivescorched.o ../src/weapons/ShieldReflective.cpp

../src/scorched/Release/srcweaponsShieldReflectiveMagscorched.o : ../src/weapons/ShieldReflectiveMag.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsShieldReflectiveMagscorched.o ../src/weapons/ShieldReflectiveMag.cpp

../src/scorched/Release/srcweaponsWeaponscorched.o : ../src/weapons/Weapon.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsWeaponscorched.o ../src/weapons/Weapon.cpp

../src/scorched/Release/srcweaponsWeaponClodscorched.o : ../src/weapons/WeaponClod.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsWeaponClodscorched.o ../src/weapons/WeaponClod.cpp

../src/scorched/Release/srcweaponsWeaponFunkyscorched.o : ../src/weapons/WeaponFunky.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsWeaponFunkyscorched.o ../src/weapons/WeaponFunky.cpp

../src/scorched/Release/srcweaponsWeaponMirvscorched.o : ../src/weapons/WeaponMirv.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsWeaponMirvscorched.o ../src/weapons/WeaponMirv.cpp

../src/scorched/Release/srcweaponsWeaponNapalmscorched.o : ../src/weapons/WeaponNapalm.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsWeaponNapalmscorched.o ../src/weapons/WeaponNapalm.cpp

../src/scorched/Release/srcweaponsWeaponProjectilescorched.o : ../src/weapons/WeaponProjectile.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsWeaponProjectilescorched.o ../src/weapons/WeaponProjectile.cpp

../src/scorched/Release/srcweaponsWeaponRiotBombscorched.o : ../src/weapons/WeaponRiotBomb.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsWeaponRiotBombscorched.o ../src/weapons/WeaponRiotBomb.cpp

../src/scorched/Release/srcweaponsWeaponTracerscorched.o : ../src/weapons/WeaponTracer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcweaponsWeaponTracerscorched.o ../src/weapons/WeaponTracer.cpp

../src/scorched/Release/srcclientClientActionsHandlerscorched.o : ../src/client/ClientActionsHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientActionsHandlerscorched.o ../src/client/ClientActionsHandler.cpp

../src/scorched/Release/srcclientClientAddPlayerHandlerscorched.o : ../src/client/ClientAddPlayerHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientAddPlayerHandlerscorched.o ../src/client/ClientAddPlayerHandler.cpp

../src/scorched/Release/srcclientClientBuyAccessoryHandlerscorched.o : ../src/client/ClientBuyAccessoryHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientBuyAccessoryHandlerscorched.o ../src/client/ClientBuyAccessoryHandler.cpp

../src/scorched/Release/srcclientClientConnectionAcceptHandlerscorched.o : ../src/client/ClientConnectionAcceptHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientConnectionAcceptHandlerscorched.o ../src/client/ClientConnectionAcceptHandler.cpp

../src/scorched/Release/srcclientClientDefenseHandlerscorched.o : ../src/client/ClientDefenseHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientDefenseHandlerscorched.o ../src/client/ClientDefenseHandler.cpp

../src/scorched/Release/srcclientClientDialogscorched.o : ../src/client/ClientDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientDialogscorched.o ../src/client/ClientDialog.cpp

../src/scorched/Release/srcclientClientGameStoppedHandlerscorched.o : ../src/client/ClientGameStoppedHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientGameStoppedHandlerscorched.o ../src/client/ClientGameStoppedHandler.cpp

../src/scorched/Release/srcclientClientMainscorched.o : ../src/client/ClientMain.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientMainscorched.o ../src/client/ClientMain.cpp

../src/scorched/Release/srcclientClientMessageHandlerscorched.o : ../src/client/ClientMessageHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientMessageHandlerscorched.o ../src/client/ClientMessageHandler.cpp

../src/scorched/Release/srcclientClientNewGameHandlerscorched.o : ../src/client/ClientNewGameHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientNewGameHandlerscorched.o ../src/client/ClientNewGameHandler.cpp

../src/scorched/Release/srcclientClientNextRoundStatescorched.o : ../src/client/ClientNextRoundState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientNextRoundStatescorched.o ../src/client/ClientNextRoundState.cpp

../src/scorched/Release/srcclientClientRmPlayerHandlerscorched.o : ../src/client/ClientRmPlayerHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientRmPlayerHandlerscorched.o ../src/client/ClientRmPlayerHandler.cpp

../src/scorched/Release/srcclientClientScoreHandlerscorched.o : ../src/client/ClientScoreHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientScoreHandlerscorched.o ../src/client/ClientScoreHandler.cpp

../src/scorched/Release/srcclientClientShotStatescorched.o : ../src/client/ClientShotState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientShotStatescorched.o ../src/client/ClientShotState.cpp

../src/scorched/Release/srcclientClientStartGameHandlerscorched.o : ../src/client/ClientStartGameHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientStartGameHandlerscorched.o ../src/client/ClientStartGameHandler.cpp

../src/scorched/Release/srcclientClientStatescorched.o : ../src/client/ClientState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientStatescorched.o ../src/client/ClientState.cpp

../src/scorched/Release/srcclientClientTextHandlerscorched.o : ../src/client/ClientTextHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientClientTextHandlerscorched.o ../src/client/ClientTextHandler.cpp

../src/scorched/Release/srcclientGLSetupscorched.o : ../src/client/GLSetup.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientGLSetupscorched.o ../src/client/GLSetup.cpp

../src/scorched/Release/srcclientMain2DCamerascorched.o : ../src/client/Main2DCamera.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientMain2DCamerascorched.o ../src/client/Main2DCamera.cpp

../src/scorched/Release/srcclientMainBannerscorched.o : ../src/client/MainBanner.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientMainBannerscorched.o ../src/client/MainBanner.cpp

../src/scorched/Release/srcclientMainCamerascorched.o : ../src/client/MainCamera.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientMainCamerascorched.o ../src/client/MainCamera.cpp

../src/scorched/Release/srcclientShotTimerscorched.o : ../src/client/ShotTimer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientShotTimerscorched.o ../src/client/ShotTimer.cpp

../src/scorched/Release/srcclientSpeedChangescorched.o : ../src/client/SpeedChange.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientSpeedChangescorched.o ../src/client/SpeedChange.cpp

../src/scorched/Release/srcclientWindowSetupscorched.o : ../src/client/WindowSetup.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o ../src/scorched/Release/srcclientWindowSetupscorched.o ../src/client/WindowSetup.cpp

# Other Deps
../src/scorched/Release/srctankaiTankAIHumanscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/common/FileLines.h ../src/tankai/TankAIHuman.cpp ../src/tank/TankWeapon.h ../src/GLEXT/GLTexture.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/tankgraph/TankModelRenderer.h ../src/ode/odemath.h ../src/common/Keyboard.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/Sound.h ../src/GLEXT/GLVertexArray.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/GLEXT/GLImage.h ../src/tank/TankModelId.h ../src/tankgraph/TankMesh.h ../src/tank/TankState.h ../src/tankgraph/TankModel.h ../src/common/SoundStore.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tankai/TankAIHuman.h ../src/tank/TankParachutes.h ../src/common/SoundBuffer.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcGLWGLWScrollHscorched.o: ../src/GLW/GLWidget.h ../src/GLW/GLWButton.h ../src/GLW/GLWScrollButton.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWVisibleWidget.h ../src/GLW/GLWScrollH.h ../src/common/KeyboardHistory.h ../src/GLW/GLWScrollH.cpp
../src/scorched/Release/srcengineActionBufferscorched.o: ../src/engine/ActionMeta.h ../src/engine/Action.h ../src/engine/ActionBuffer.cpp ../src/engine/ActionBuffer.h ../src/coms/NetBuffer.h
../src/scorched/Release/srctankTankScorescorched.o: ../src/common/FileLines.h ../src/common/Defines.h ../src/common/OptionEntry.h ../src/common/OptionsGame.h ../src/coms/NetBuffer.h ../src/tank/TankScore.cpp ../src/common/ARGParser.h ../src/tank/TankScore.h
../src/scorched/Release/srcspritesDebrisActionRendererscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/ode/contact.h ../src/ode/rotation.h ../src/engine/PhysicsParticle.h ../src/ode/memory.h ../src/engine/Action.h ../src/sprites/DebrisActionRenderer.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/common/Defines.h ../src/3dsparse/ASEStore.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/GLEXT/GLVertexArray.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/ode/collision.h ../src/ode/space.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/coms/NetBuffer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/ode/config.h ../src/ode/odecpp.h ../src/sprites/DebrisActionRenderer.cpp
../src/scorched/Release/srcspritesTankDeadRendererscorched.o: ../src/engine/Action.h ../src/sprites/TankDeadRenderer.h ../src/common/Defines.h ../src/common/Sound.h ../src/sprites/TankDeadRenderer.cpp ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/NetBuffer.h ../src/common/SoundStore.h ../src/common/SoundBuffer.h
../src/scorched/Release/srcactionsShotProjectileMirvscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/ode/contact.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/ode/memory.h ../src/engine/Action.h ../src/actions/ShotProjectileMirv.cpp ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/actions/ShotProjectileExplosion.h ../src/ode/misc.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/engine/MainLoopI.h ../src/actions/ShotProjectileMirv.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/ode/config.h ../src/ode/odecpp.h ../src/engine/ActionController.h
../src/scorched/Release/srczlibadler32scorched.o: ../src/zlib/adler32.c
../src/scorched/Release/srcspritesSmokeActionRendererscorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/sprites/SmokeActionRenderer.h ../src/landscape/PatchGrid.h ../src/ode/contact.h ../src/GLEXT/GLTexture.h ../src/ode/rotation.h ../src/engine/PhysicsParticle.h ../src/landscape/WaterMapSurround.h ../src/ode/memory.h ../src/engine/Action.h ../src/landscape/Landscape.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/common/Defines.h ../src/ode/mass.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/landscape/SurroundDefs.h ../src/ode/collision_space.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/landscape/Patch.h ../src/engine/MainLoopI.h ../src/landscape/TriNode.h ../src/landscape/HeightMapSurround.h ../src/ode/collision.h ../src/ode/space.h ../src/landscape/WaterMapPoints.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/landscape/ShadowMap.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/sprites/SmokeActionRenderer.cpp ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/landscape/PatchSide.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/GLEXT/GLTextureCubeMap.h ../src/landscape/WaterMap.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/ode/odecpp.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcGLEXTGLConsoleMethodsscorched.o: ../src/common/Vector.h ../src/GLEXT/GLTexture.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLEXT/GLConsoleMethods.cpp ../src/GLEXT/GLConsoleRuleMethod.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLConsoleFileReader.h ../src/GLEXT/GLConsoleMethods.h ../src/GLEXT/GLConsoleRule.h ../src/GLEXT/GLFont2d.h ../src/GLEXT/GLConsoleLines.h ../src/GLEXT/GLConsoleRules.h
../src/scorched/Release/srcweaponsShieldReflectivescorched.o: ../src/common/Vector.h ../src/weapons/ShieldReflective.h ../src/weapons/ShieldReflective.cpp ../src/weapons/Accessory.h ../src/weapons/Shield.h
../src/scorched/Release/srctankTankControllerscorched.o: ../src/engine/ActionMeta.h ../src/tank/TankController.cpp ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/tank/TankController.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/actions/TankDamage.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/engine/ActionController.h ../src/tank/TankScore.h
../src/scorched/Release/srcclientClientTextHandlerscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/tank/TankShields.h ../src/GLEXT/GLTexture.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/sprites/TalkRenderer.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/client/ClientTextHandler.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/client/ClientTextHandler.cpp ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/coms/ComsTextMessage.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/GLEXT/GLImage.h ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/client/ClientState.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h ../src/engine/ActionController.h
../src/scorched/Release/srcGLEXTGLConsoleRuleMethodscorched.o: ../src/GLEXT/GLConsoleRuleMethod.h ../src/GLEXT/GLConsoleRuleMethod.cpp ../src/GLEXT/GLConsoleRule.h
../src/scorched/Release/src3dsparseMeshLODVectorscorched.o: ../src/common/Vector.h ../src/common/Defines.h ../src/3dsparse/MeshLODVector.cpp ../src/3dsparse/MeshLODTri.h ../src/3dsparse/MeshLODVector.h
../src/scorched/Release/srcweaponsWeaponMirvscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/PhysicsParticle.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/actions/ShotProjectileMirv.h ../src/tank/TankBatteries.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/WeaponMirv.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/weapons/WeaponMirv.cpp ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcscorchedServerSDialogscorched.o: ../src/scorched/MainDialog.h ../src/common/FileLines.h ../src/common/Defines.h ../src/scorched/SettingsDialog.h ../src/server/ServerMain.h ../src/common/OptionEntry.h ../src/coms/NetMessage.h ../src/common/OptionsParam.h ../src/common/OptionsGame.h ../src/scorched//ServerSDialog.cpp ../src/coms/NetBuffer.h ../src/common/ARGParser.h ../src/scorched/ServerDialog.h
../src/scorched/Release/srcdialogsWindDialogscorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/common/Vector.h ../src/3dsparse/ASEFile.h ../src/GLEXT/GLCamera.h ../src/landscape/PatchGrid.h ../src/GLW/GLWidget.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/dialogs/WindDialog.h ../src/GLEXT/GLTexture.h ../src/landscape/WaterMapSurround.h ../src/common/Line.h ../src/GLEXT/GLMenuI.h ../src/landscape/Landscape.h ../src/common/Face.h ../src/engine/GameStateI.h ../src/dialogs/WindDialog.cpp ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/GLEXT/GLVertexTexArray.h ../src/common/Defines.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/client/MainCamera.h ../src/landscape/SurroundDefs.h ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLVertexArray.h ../src/GLEXT/GLState.h ../src/landscape/Patch.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/landscape/TriNode.h ../src/landscape/HeightMapSurround.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/coms/ComsMessage.h ../src/landscape/WaterMapPoints.h ../src/landscape/Smoke.h ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/3dsparse/ASEModel.h ../src/landscape/ShadowMap.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLImage.h ../src/3dsparse/ASEArrayFact.h ../src/common/ProgressCounter.h ../src/landscape/PatchSide.h ../src/coms/ComsLevelMessage.h ../src/GLEXT/GLFont2d.h ../src/GLEXT/GLTextureCubeMap.h ../src/landscape/WaterMap.h ../src/GLW/GLWWindow.h ../src/common/OptionsTransient.h ../src/landscape/MovementMap.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcclientClientNextRoundStatescorched.o: ../src/common/Vector.h ../src/common/FileLines.h ../src/client/ClientStartGameHandler.h ../src/client/ClientNextRoundState.h ../src/coms/ComsMessageHandler.h ../src/engine/GameStateI.h ../src/coms/ComsMessageSender.h ../src/common/OptionEntry.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/common/OptionsParam.h ../src/coms/ComsMessage.h ../src/coms/ComsPlayerReadyMessage.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/coms/NetMessageHandler.h ../src/client/ClientState.h ../src/client/ClientNextRoundState.cpp ../src/common/ARGParser.h
../src/scorched/Release/srcweaponsWeaponscorched.o: ../src/common/Defines.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Weapon.cpp
../src/scorched/Release/srcserverServerShotHolderscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/coms/ComsDefenseMessage.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/server/ServerShotHolder.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/coms/ComsPlayedMoveMessage.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/server/ServerShotHolder.cpp ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankAILogic.h ../src/tank/TankScore.h
../src/scorched/Release/srcactionsShotProjectileFunkyscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/actions/ShotProjectileFunky.cpp ../src/engine/PhysicsParticle.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/tank/TankLib.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/actions/ShotProjectileExplosion.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/actions/ShotProjectileFunky.h ../src/ode/config.h ../src/ode/odecpp.h ../src/engine/ActionController.h ../src/tank/TankScore.h
../src/scorched/Release/srclandscapeHeightMapCollisionscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/ode/contact.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/landscape/HeightMapCollision.h ../src/common/Line.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/engine/MainLoopI.h ../src/ode/collision.h ../src/ode/space.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/common/ProgressCounter.h ../src/common/Triangle.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/landscape/HeightMapCollision.cpp ../src/ode/config.h ../src/ode/odecpp.h ../src/engine/ActionController.h
../src/scorched/Release/srczlibinftreesscorched.o: ../src/zlib/inftrees.c
../src/scorched/Release/srccommonOptionEntryscorched.o: ../src/XML/XMLFile.h ../src/common/FileLines.h ../src/XML/XMLParser.h ../src/common/OptionEntry.cpp ../src/common/Defines.h ../src/common/OptionEntry.h ../src/coms/NetBuffer.h ../src/common/ARGParser.h ../src/XML/expat.h
../src/scorched/Release/srcGLEXTGLBitmapscorched.o: ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLEXT/GLBitmap.cpp ../src/GLEXT/GLImage.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcweaponsAccessoryStorescorched.o: ../src/common/Vector.h ../src/weapons/ShieldReflective.h ../src/weapons/WeaponProjectile.h ../src/weapons/Parachute.h ../src/common/Defines.h ../src/weapons/WeaponTracer.h ../src/weapons/AccessoryStore.cpp ../src/weapons/ShieldReflectiveMag.h ../src/weapons/Battery.h ../src/weapons/WeaponMirv.h ../src/weapons/Weapon.h ../src/weapons/WeaponClod.h ../src/weapons/Accessory.h ../src/coms/NetBuffer.h ../src/weapons/WeaponFunky.h ../src/weapons/Shield.h ../src/weapons/AccessoryStore.h ../src/weapons/WeaponNapalm.h ../src/weapons/WeaponRiotBomb.h ../src/weapons/Fuel.h ../src/weapons/AutoDefense.h
../src/scorched/Release/srctankgraphTankModelRendererscorched.o: ../src/landscape/TriNodePool.h ../src/engine/ActionMeta.h ../src/landscape/Wall.h ../src/actions/TankFalling.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/GLEXT/GLCamera.h ../src/landscape/PatchGrid.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/GLEXT/GLTexture.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/PhysicsParticle.h ../src/engine/ScorchedCollisionIds.h ../src/landscape/WaterMapSurround.h ../src/tank/TankAutoDefense.h ../src/common/Line.h ../src/ode/memory.h ../src/engine/Action.h ../src/GLEXT/GLMenuI.h ../src/landscape/Hemisphere.h ../src/landscape/Landscape.h ../src/tankgraph/TankModelRenderer.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/tankgraph/TankModelStore.h ../src/ode/mass.h ../src/client/MainCamera.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/landscape/SurroundDefs.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/GLEXT/GLVertexArray.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/landscape/Patch.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/landscape/TriNode.h ../src/landscape/HeightMapSurround.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/landscape/WaterMapPoints.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/landscape/Smoke.h ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/landscape/ShadowMap.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLCameraFrustum.h ../src/weapons/Shield.h ../src/GLEXT/GLImage.h ../src/tank/TankModelId.h ../src/tankgraph/TankMesh.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/tankgraph/TankModel.h ../src/landscape/PatchSide.h ../src/ode/geom.h ../src/coms/ComsLevelMessage.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/GLEXT/GLFont2d.h ../src/engine/PhysicsParticleObject.h ../src/GLEXT/GLTextureCubeMap.h ../src/tank/TankFuel.h ../src/landscape/WaterMap.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/common/ARGParser.h ../src/tankgraph/TankModelRenderer.cpp ../src/ode/odecpp.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h ../src/landscape/MovementMap.h ../src/tank/TankScore.h
../src/scorched/Release/srcGLEXTGLCameraFrustumscorched.o: ../src/common/Vector.h ../src/engine/GameStateI.h ../src/common/Defines.h ../src/GLEXT/GLCameraFrustum.cpp ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/GLEXT/GLCameraFrustum.h
../src/scorched/Release/srcGLWGLWSpinBoxscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/GLEXT/GLTexture.h ../src/GLW/GLWButton.h ../src/GLW/GLWSpinBox.cpp ../src/common/Keyboard.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWVisibleWidget.h ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLFont2d.h ../src/GLW/GLWSpinBox.h
../src/scorched/Release/srccomsNetMessagescorched.o: ../src/coms/NetMessage.cpp ../src/coms/NetMessage.h ../src/coms/NetBuffer.h
../src/scorched/Release/srcGLWGLWDropDownscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/GLEXT/GLTexture.h ../src/GLW/GLWButton.h ../src/GLW/GLWDropDown.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWDropDown.cpp ../src/GLW/GLWVisibleWidget.h ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/GLW/GLWPushButton.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLFont2d.h
../src/scorched/Release/srcserverServerPlayingStatescorched.o: ../src/common/Vector.h ../src/server/ServerPlayingState.cpp ../src/tank/TankPhysics.h ../src/engine/GameStateStimulusI.h ../src/ode/contact.h ../src/common/FileLines.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/server/ServerShotHolder.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/common/OptionsGame.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/coms/ComsPlayedMoveMessage.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/server/ServerPlayingState.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/common/ARGParser.h ../src/coms/ComsStartGameMessage.h ../src/ode/odecpp.h ../src/coms/ComsPlayerStateMessage.h ../src/tank/TankScore.h
../src/scorched/Release/srcspritesTextActionRendererscorched.o: ../src/common/Vector.h ../src/sprites/TextActionRenderer.h ../src/GLEXT/GLTexture.h ../src/engine/Action.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWFont.h ../src/GLEXT/GLImage.h ../src/sprites/TextActionRenderer.cpp ../src/GLEXT/GLFont2d.h
../src/scorched/Release/srccomsComsScoreMessagescorched.o: ../src/coms/ComsScoreMessage.h ../src/coms/ComsScoreMessage.cpp ../src/coms/NetMessage.h ../src/coms/ComsMessage.h ../src/coms/NetBuffer.h
../src/scorched/Release/srccomsComsBuyAccessoryMessagescorched.o: ../src/coms/ComsBuyAccessoryMessage.cpp ../src/coms/NetMessage.h ../src/coms/ComsMessage.h ../src/coms/NetBuffer.h ../src/coms/ComsBuyAccessoryMessage.h
../src/scorched/Release/srcactionsWallHitscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/common/FileLines.h ../src/actions/WallHit.h ../src/GLEXT/GLTexture.h ../src/sprites/WallActionRenderer.h ../src/engine/Action.h ../src/common/Defines.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/common/OptionsParam.h ../src/coms/NetBuffer.h ../src/GLEXT/GLImage.h ../src/actions/WallHit.cpp ../src/common/OptionsTransient.h ../src/common/ARGParser.h
../src/scorched/Release/srcoderotationscorched.o: ../src/ode/rotation.h ../src/ode/odemath.h ../src/ode/common.h ../src/ode/error.h ../src/ode/rotation.cpp ../src/ode/config.h
../src/scorched/Release/srcserverServerTooFewPlayersStimulusscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/engine/GameStateStimulusI.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/server/ServerTooFewPlayersStimulus.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/coms/ComsGameStoppedMessage.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/ode/matrix.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/server/ServerTooFewPlayersStimulus.cpp ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/common/ARGParser.h ../src/scorched/ServerDialog.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcactionsTankFiredscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/GLEXT/GLTexture.h ../src/tank/TankShields.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/tankgraph/TankModelRenderer.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/Sound.h ../src/GLEXT/GLVertexArray.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/common/OptionsParam.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/actions/TankFired.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/engine/GameState.h ../src/actions/Explosion.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/weapons/AccessoryStore.h ../src/GLEXT/GLImage.h ../src/tank/TankModelId.h ../src/tankgraph/TankMesh.h ../src/tank/TankState.h ../src/tankgraph/TankModel.h ../src/common/SoundStore.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/actions/TankFired.cpp ../src/tank/TankFuel.h ../src/common/SoundBuffer.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/tank/TankScore.h ../src/engine/ActionController.h
../src/scorched/Release/srcGLWGLWTabscorched.o: ../src/GLW/GLWidget.h ../src/GLW/GLWTab.h ../src/GLW/GLWButton.h ../src/GLW/GLWScrollW.h ../src/GLW/GLWScrollButton.h ../src/GLW/GLWTab.cpp ../src/common/Defines.h ../src/GLW/GLWVisiblePanel.h ../src/GLW/GLWLabel.h ../src/GLEXT/GLState.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h ../src/GLW/GLWScrollPanel.h
../src/scorched/Release/srclandscapeWaterMapscorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/common/Vector.h ../src/landscape/PatchGrid.h ../src/common/FileLines.h ../src/GLEXT/GLTexture.h ../src/landscape/WaterMapSurround.h ../src/landscape/Landscape.h ../src/engine/GameStateI.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/landscape/SurroundDefs.h ../src/common/OptionEntry.h ../src/GLEXT/GLStateExtension.h ../src/GLEXT/GLState.h ../src/landscape/Patch.h ../src/engine/MainLoopI.h ../src/landscape/TriNode.h ../src/landscape/HeightMapSurround.h ../src/landscape/WaterMap.cpp ../src/landscape/WaterMapPoints.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/landscape/ShadowMap.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLCameraFrustum.h ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/landscape/PatchSide.h ../src/common/Resources.h ../src/GLEXT/GLTextureCubeMap.h ../src/landscape/WaterMap.h ../src/common/OptionsTransient.h ../src/common/ARGParser.h ../src/common/ResourceFile.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcGLWGLWFileViewscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/common/FileLines.h ../src/GLEXT/GLTexture.h ../src/GLW/GLWButton.h ../src/GLW/GLWScrollW.h ../src/GLW/GLWScrollButton.h ../src/common/Defines.h ../src/GLW/GLWFileView.h ../src/GLEXT/GLState.h ../src/GLW/GLWVisibleWidget.h ../src/GLW/GLWFileView.cpp ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLFont2d.h
../src/scorched/Release/srcserverServerTextHandlerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/coms/ComsTextMessage.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/server/ServerTextHandler.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/server/ServerTextHandler.cpp ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srccommonResourceFilescorched.o: ../src/XML/XMLFile.h ../src/common/ResourceFile.cpp ../src/XML/XMLParser.h ../src/common/Defines.h ../src/common/ResourceFile.h ../src/XML/expat.h
../src/scorched/Release/srcclientClientAddPlayerHandlerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/client/ClientAddPlayerHandler.cpp ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/client/ClientConnectionAcceptHandler.h ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/coms/ComsAddPlayerMessage.h ../src/tank/TankContainer.h ../src/client/ClientState.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tankai/TankAIHuman.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/client/ClientAddPlayerHandler.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcactionsExplosionscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/GLEXT/GLTexture.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/landscape/DeformTextures.h ../src/engine/PhysicsParticle.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/landscape/DeformLandscape.h ../src/engine/Action.h ../src/sprites/ExplosionNukeRenderer.h ../src/weapons/WeaponProjectile.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/tank/TankController.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/actions/Explosion.cpp ../src/ode/collision_space.h ../src/common/Sound.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/common/OptionsParam.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/sprites/ExplosionTextures.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/actions/Explosion.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/GLEXT/GLImage.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/common/SoundStore.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/sprites/ExplosionRenderer.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/tank/TankFuel.h ../src/common/SoundBuffer.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/GLEXT/GLTextureSet.h ../src/engine/ActionController.h ../src/tank/TankScore.h
../src/scorched/Release/srcserverServerNewGameStatescorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/ode/contact.h ../src/common/FileLines.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/server/ServerNewGameState.cpp ../src/tank/TankAutoDefense.h ../src/server/ServerState.h ../src/ode/memory.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/common/Timer.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/common/OptionsGame.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/coms/ComsLevelMessage.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankStart.h ../src/tank/TankParachutes.h ../src/coms/ComsNewGameMessage.h ../src/ode/config.h ../src/common/ARGParser.h ../src/scorched/ServerDialog.h ../src/ode/odecpp.h ../src/server/ServerNewGameState.h ../src/landscape/MovementMap.h ../src/tank/TankScore.h
../src/scorched/Release/srcdialogsPlayersDialogscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/client/ClientNewGameHandler.h ../src/GLW/GLWidget.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/GLW/GLWFlag.h ../src/tank/TankAutoDefense.h ../src/GLW/GLWButton.h ../src/ode/memory.h ../src/dialogs/PlayersDialog.h ../src/GLEXT/GLMenuI.h ../src/GLW/GLWScrollW.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/GLW/GLWScrollButton.h ../src/GLW/GLWDropDown.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/dialogs/PlayersDialog.cpp ../src/ode/collision_space.h ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLVertexArray.h ../src/common/OptionEntry.h ../src/GLW/GLWLabel.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tankai/TankAIStore.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/tank/TankColorGenerator.h ../src/GLW/GLWPanel.h ../src/ode/collision.h ../src/ode/space.h ../src/GLW/GLWVisibleWidget.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/tankgraph/GLWTankViewer.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/GLW/GLWPushButton.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/coms/NetMessageHandler.h ../src/common/WindowManager.h ../src/tank/TankState.h ../src/client/ClientState.h ../src/tankai/TankAIComputer.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/GLW/GLWTextBox.h ../src/ode/objects.h ../src/ode/error.h ../src/tankai/TankAIComputerBuyer.h ../src/tank/TankFuel.h ../src/tankai/TankAIHuman.h ../src/GLW/GLWWindow.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/common/ARGParser.h ../src/tankai/TankAIAdder.h ../src/ode/odecpp.h ../src/GLW/GLWTextButton.h ../src/tank/TankScore.h
../src/scorched/Release/srctankaiTankAIComputerCtrlscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/server/ServerShotHolder.h ../src/engine/ScorchedCollisionIds.h ../src/server/ServerState.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/tankai/TankAIComputerCtrl.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/coms/ComsPlayedMoveMessage.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/tankai/TankAIComputerCtrl.cpp ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srclandscapeVariancescorched.o: ../src/common/Vector.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/landscape/Variance.cpp ../src/common/ProgressCounter.h ../src/landscape/Variance.h
../src/scorched/Release/srczlibuncomprscorched.o: ../src/zlib/uncompr.c
../src/scorched/Release/srcserverServerNextRoundStatescorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/engine/GameStateStimulusI.h ../src/ode/contact.h ../src/common/FileLines.h ../src/tank/TankWeapon.h ../src/tank/TankShields.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/server/ServerState.h ../src/ode/memory.h ../src/server/ServerNextRoundState.cpp ../src/engine/Action.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/server/ServerNextRoundState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/common/OptionsGame.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankStart.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/common/ARGParser.h ../src/scorched/ServerDialog.h ../src/ode/odecpp.h ../src/server/ServerNewGameState.h ../src/tank/TankScore.h ../src/engine/ActionController.h
../src/scorched/Release/srcdialogsQuitDialogscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/GLW/GLWButton.h ../src/dialogs/QuitDialog.cpp ../src/GLEXT/GLMenuI.h ../src/engine/GameStateI.h ../src/common/Defines.h ../src/GLW/GLWVisiblePanel.h ../src/GLW/GLWLabel.h ../src/dialogs/QuitDialog.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/common/Timer.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/common/WindowManager.h ../src/GLW/GLWWindow.h ../src/engine/MainLoop.h ../src/GLW/GLWTextButton.h
../src/scorched/Release/srccomsNetClientscorched.o: ../src/coms/NetClient.cpp ../src/coms/NetClient.h ../src/coms/NetMessage.h ../src/coms/NetBuffer.h ../src/coms/NetMessageHandler.h ../src/common/Logger.h ../src/coms/NetBufferUtil.h ../src/coms/NetMessagePool.h
../src/scorched/Release/srcdialogsPlanViewDialogscorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/GLEXT/GLCamera.h ../src/landscape/PatchGrid.h ../src/GLW/GLWidget.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/GLEXT/GLTexture.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/landscape/WaterMapSurround.h ../src/tank/TankAutoDefense.h ../src/common/Line.h ../src/ode/memory.h ../src/GLEXT/GLMenuI.h ../src/landscape/Landscape.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/client/MainCamera.h ../src/landscape/SurroundDefs.h ../src/ode/collision_space.h ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLState.h ../src/dialogs/PlanViewDialog.h ../src/ode/timer.h ../src/landscape/Patch.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/landscape/TriNode.h ../src/landscape/HeightMapSurround.h ../src/GLW/GLWPanel.h ../src/ode/collision.h ../src/ode/space.h ../src/GLW/GLWVisibleWidget.h ../src/weapons/Weapon.h ../src/landscape/WaterMapPoints.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/landscape/ShadowMap.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/dialogs/PlanViewDialog.cpp ../src/weapons/Shield.h ../src/GLEXT/GLImage.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/tank/TankContainer.h ../src/landscape/PatchSide.h ../src/ode/geom.h ../src/coms/ComsLevelMessage.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/GLEXT/GLTextureCubeMap.h ../src/tank/TankFuel.h ../src/landscape/WaterMap.h ../src/GLW/GLWWindow.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/ode/odecpp.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h ../src/landscape/MovementMap.h ../src/tank/TankScore.h
../src/scorched/Release/srcdialogsBackdropDialogscorched.o: ../src/GLW/GLWidget.h ../src/GLEXT/GLTexture.h ../src/common/Defines.h ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLState.h ../src/dialogs/BackdropDialog.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h ../src/dialogs/BackdropDialog.cpp ../src/GLEXT/GLImage.h ../src/GLW/GLWWindow.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srclandscapeWallscorched.o: ../src/landscape/Wall.h ../src/common/Vector.h ../src/GLEXT/GLTexture.h ../src/landscape/Wall.cpp ../src/common/Defines.h ../src/common/Sound.h ../src/GLEXT/GLState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLImage.h ../src/common/SoundStore.h ../src/common/SoundBuffer.h ../src/common/OptionsTransient.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srctankTankPhysicsscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/tank/TankShields.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/tank/TankLib.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/tank/TankPhysics.cpp ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/engine/ActionController.h ../src/tank/TankScore.h
../src/scorched/Release/srctankTankBatteriesscorched.o: ../src/common/Defines.h ../src/tank/TankBatteries.h ../src/coms/NetBuffer.h ../src/tank/TankBatteries.cpp
../src/scorched/Release/srctankaiTankAIComputerRandomscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tankai/TankAIComputerRandom.cpp ../src/tank/Tank.h ../src/tankai/TankAIStore.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tankai/TankAIComputer.h ../src/ode/geom.h ../src/tankai/TankAIComputerRandom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tankai/TankAIComputerBuyer.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srclandscapeHeightMapscorched.o: ../src/common/Vector.h ../src/landscape/HeightMap.cpp ../src/common/Line.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/common/ProgressCounter.h ../src/common/Triangle.h
../src/scorched/Release/src3dsparseaseFile.tabscorched.o: ../src/common/Vector.h ../src/3dsparse/ASEFile.h ../src/3dsparse/aseFile.tab.cpp ../src/common/Face.h ../src/common/Defines.h ../src/3dsparse/ASEModel.h
../src/scorched/Release/srcengineScorchedCollisionHandlerscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/actions/WallHit.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/PhysicsParticle.h ../src/engine/ScorchedCollisionIds.h ../src/actions/ShieldHit.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/engine/ScorchedCollisionHandler.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/engine/ScorchedCollisionHandler.cpp ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/ode/odecpp.h ../src/engine/ActionController.h ../src/tank/TankScore.h
../src/scorched/Release/srcGLWGLWFontscorched.o: ../src/common/Vector.h ../src/GLEXT/GLTexture.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWFont.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLFont2d.h ../src/GLW/GLWFont.cpp
../src/scorched/Release/srcclientClientDialogscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/GLEXT/GLCamera.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/client/GLSetup.h ../src/ode/contact.h ../src/common/FileLines.h ../src/tank/TankWeapon.h ../src/tank/TankShields.h ../src/GLEXT/GLTexture.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/landscape/HeightMapCollision.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/common/Line.h ../src/engine/Action.h ../src/GLEXT/GLMenuI.h ../src/ode/odemath.h ../src/common/Keyboard.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/client/ClientDialog.cpp ../src/engine/ScorchedCollisionHandler.h ../src/common/OptionsDisplay.h ../src/tankgraph/TankModelStore.h ../src/ode/mass.h ../src/common/Gamma.h ../src/client/MainCamera.h ../src/ode/collision_space.h ../src/common/Sound.h ../src/GLEXT/GLVertexArray.h ../src/common/OptionEntry.h ../src/GLEXT/GLStateExtension.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/common/OptionsParam.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/common/Timer.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/client/ClientDialog.h ../src/engine/PhysicsEngine.h ../src/common/OptionsGame.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/GLEXT/GLImage.h ../src/tank/TankModelId.h ../src/tankgraph/TankMesh.h ../src/GLEXT/GLConsoleFileReader.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/tankgraph/TankModel.h ../src/common/Display.h ../src/client/ClientState.h ../src/ode/geom.h ../src/coms/ComsLevelMessage.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/client/WindowSetup.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/landscape/MovementMap.h ../src/engine/MainLoop.h ../src/tank/TankScore.h ../src/engine/ActionController.h
../src/scorched/Release/srctankTankLibscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/tank/TankLib.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/tank/TankLib.cpp ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcspritesExplosionNukeRendererscorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/common/Vector.h ../src/landscape/PatchGrid.h ../src/GLEXT/GLTexture.h ../src/landscape/WaterMapSurround.h ../src/engine/Action.h ../src/sprites/ExplosionNukeRenderer.h ../src/landscape/Landscape.h ../src/engine/GameStateI.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/landscape/SurroundDefs.h ../src/sprites/ExplosionNukeRenderer.cpp ../src/GLEXT/GLState.h ../src/landscape/Patch.h ../src/engine/MainLoopI.h ../src/landscape/TriNode.h ../src/landscape/HeightMapSurround.h ../src/landscape/WaterMapPoints.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/landscape/ShadowMap.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/landscape/PatchSide.h ../src/GLEXT/GLTextureCubeMap.h ../src/landscape/WaterMap.h ../src/common/OptionsTransient.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcGLEXTGLStateExtensionscorched.o: ../src/common/Defines.h ../src/GLEXT/GLStateExtension.h ../src/GLEXT/GLState.h ../src/GLEXT/GLStateExtension.cpp
../src/scorched/Release/srcclientClientScoreHandlerscorched.o: ../src/common/Vector.h ../src/coms/ComsScoreMessage.h ../src/coms/ComsMessageHandler.h ../src/client/ClientScoreHandler.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/coms/ComsMessage.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/coms/NetMessageHandler.h ../src/client/ClientState.h ../src/client/ClientScoreHandler.cpp
../src/scorched/Release/srcodejointscorched.o: ../src/ode/rotation.h ../src/ode/odemath.h ../src/ode/joint.cpp ../src/ode/matrix.h ../src/ode/common.h ../src/ode/error.h ../src/ode/config.h
../src/scorched/Release/srctankTankscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/tank/Tank.cpp ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/tank/TankColorGenerator.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcclientClientShotStatescorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/engine/GameStateStimulusI.h ../src/landscape/PatchGrid.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/tank/TankShields.h ../src/GLEXT/GLTexture.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/landscape/WaterMapSurround.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/landscape/Landscape.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/landscape/SurroundDefs.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/landscape/Patch.h ../src/tank/Tank.h ../src/client/ClientShotState.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/landscape/TriNode.h ../src/landscape/HeightMapSurround.h ../src/ode/collision.h ../src/ode/space.h ../src/common/OptionsParam.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/landscape/WaterMapPoints.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/landscape/ShadowMap.h ../src/engine/PhysicsEngine.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/client/ClientShotState.cpp ../src/tank/TankModelId.h ../src/GLEXT/GLImage.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/tank/TankContainer.h ../src/landscape/PatchSide.h ../src/ode/geom.h ../src/coms/ComsLevelMessage.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/GLEXT/GLTextureCubeMap.h ../src/tank/TankFuel.h ../src/tank/TankStart.h ../src/tank/TankParachutes.h ../src/landscape/WaterMap.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/landscape/MovementMap.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h ../src/tank/TankScore.h ../src/engine/ActionController.h
../src/scorched/Release/srcXMLxmlparsescorched.o: ../src/XML/expat_config.h ../src/XML/xmlparse.c
../src/scorched/Release/srctankTankWeaponscorched.o: ../src/tank/TankWeapon.h ../src/common/Defines.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/tank/TankWeapon.cpp
../src/scorched/Release/srcclientClientRmPlayerHandlerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/client/ClientRmPlayerHandler.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/coms/ComsRmPlayerMessage.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/client/ClientState.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/client/ClientRmPlayerHandler.cpp ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcweaponsFuelscorched.o: ../src/weapons/Fuel.cpp ../src/common/Defines.h ../src/weapons/Accessory.h ../src/weapons/Fuel.h
../src/scorched/Release/srccommonLoggerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/common/Logger.cpp ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srclandscapeSmokescorched.o: ../src/common/Vector.h ../src/GLEXT/GLTexture.h ../src/engine/GameStateI.h ../src/landscape/Smoke.cpp ../src/common/Defines.h ../src/landscape/SmokeChain.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/GLEXT/GLCameraFrustum.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcGLWGLWListViewscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/common/FileLines.h ../src/GLW/GLWListView.cpp ../src/GLEXT/GLTexture.h ../src/GLW/GLWButton.h ../src/GLW/GLWScrollW.h ../src/GLW/GLWScrollButton.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWVisibleWidget.h ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLFont2d.h ../src/GLW/GLWListView.h
../src/scorched/Release/srcclientMainCamerascorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/GLEXT/GLCamera.h ../src/landscape/PatchGrid.h ../src/GLW/GLWidget.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/tank/TankShields.h ../src/ode/rotation.h ../src/GLEXT/GLTexture.h ../src/dialogs/MainMenuDialog.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/landscape/WaterMapSurround.h ../src/tank/TankAutoDefense.h ../src/common/Line.h ../src/ode/memory.h ../src/engine/Action.h ../src/GLEXT/GLMenuI.h ../src/landscape/Landscape.h ../src/GLEXT/GLConsoleRuleFn.h ../src/common/Keyboard.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/GLEXT/GLMenuEntry.h ../src/ode/odecpp_collision.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/client/MainCamera.h ../src/landscape/SurroundDefs.h ../src/ode/collision_space.h ../src/common/Sound.h ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/landscape/Patch.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/landscape/TriNode.h ../src/actions/ShotProjectile.h ../src/GLEXT/GLConsoleRuleMethod.h ../src/landscape/HeightMapSurround.h ../src/GLW/GLWPanel.h ../src/ode/collision.h ../src/ode/space.h ../src/GLW/GLWVisibleWidget.h ../src/weapons/Weapon.h ../src/client/MainCamera.cpp ../src/landscape/WaterMapPoints.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/landscape/Smoke.h ../src/GLEXT/GLConsole.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/landscape/ShadowMap.h ../src/engine/PhysicsEngine.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/GLEXT/GLImage.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/GLEXT/GLConsoleMethods.h ../src/common/SoundStore.h ../src/tank/TankContainer.h ../src/landscape/PatchSide.h ../src/ode/geom.h ../src/coms/ComsLevelMessage.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/GLEXT/GLConsoleRule.h ../src/engine/PhysicsParticleObject.h ../src/GLEXT/GLFont2d.h ../src/GLEXT/GLTextureCubeMap.h ../src/GLEXT/GLConsoleLines.h ../src/tank/TankFuel.h ../src/common/SoundBuffer.h ../src/landscape/WaterMap.h ../src/tank/TankParachutes.h ../src/GLW/GLWWindow.h ../src/GLEXT/GLMenu.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/ode/odecpp.h ../src/landscape/Variance.h ../src/landscape/MovementMap.h ../src/GLEXT/GLBitmap.h ../src/GLEXT/GLConsoleRules.h ../src/tank/TankScore.h ../src/engine/ActionController.h
../src/scorched/Release/srcactionsShieldHitscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/common/FileLines.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/actions/ShieldHit.h ../src/ode/memory.h ../src/engine/Action.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/actions/ShieldHit.cpp ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/Sound.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/common/OptionsParam.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/common/SoundStore.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/common/SoundBuffer.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcdialogsBuyAccessoryDialogscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/GLW/GLWidget.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/dialogs/BuyAccessoryDialog.cpp ../src/GLW/GLWTab.h ../src/engine/ScorchedCollisionIds.h ../src/GLW/GLWFlag.h ../src/tank/TankAutoDefense.h ../src/GLW/GLWButton.h ../src/ode/memory.h ../src/GLEXT/GLMenuI.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/GLW/GLWScrollW.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/GLW/GLWScrollButton.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/GLW/GLWVisiblePanel.h ../src/common/OptionEntry.h ../src/GLW/GLWLabel.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/GLW/GLWPanel.h ../src/common/OptionsParam.h ../src/GLW/GLWVisibleWidget.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/common/WindowManager.h ../src/tank/TankState.h ../src/client/ClientState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/coms/ComsBuyAccessoryMessage.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/GLW/GLWWindow.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/dialogs/BuyAccessoryDialog.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/GLW/GLWScrollPanel.h ../src/GLW/GLWTextButton.h ../src/tank/TankScore.h
../src/scorched/Release/srccomsNetBufferUtilscorched.o: ../src/coms/NetBufferUtil.cpp ../src/coms/NetMessage.h ../src/coms/NetBuffer.h ../src/common/Logger.h ../src/coms/NetBufferUtil.h ../src/coms/NetMessagePool.h
../src/scorched/Release/srccommonVectorLibscorched.o: ../src/common/Vector.h ../src/common/Defines.h ../src/common/VectorLib.h ../src/common/VectorLib.cpp
../src/scorched/Release/srccomsComsRmPlayerMessagescorched.o: ../src/coms/ComsRmPlayerMessage.h ../src/coms/NetMessage.h ../src/coms/ComsMessage.h ../src/coms/ComsRmPlayerMessage.cpp ../src/coms/NetBuffer.h
../src/scorched/Release/srcclientSpeedChangescorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/ode/contact.h ../src/GLEXT/GLTexture.h ../src/ode/rotation.h ../src/ode/memory.h ../src/engine/Action.h ../src/ode/odemath.h ../src/common/Keyboard.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/client/SpeedChange.cpp ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/engine/MainLoopI.h ../src/ode/collision.h ../src/ode/space.h ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/client/SpeedChange.h ../src/GLEXT/GLImage.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/GLEXT/GLFont2d.h ../src/ode/config.h ../src/ode/odecpp.h ../src/engine/ActionController.h
../src/scorched/Release/srcserverServerConnectHandlerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/server/ServerState.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/coms/ComsConnectMessage.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/tank/TankColorGenerator.h ../src/coms/ComsConnectAcceptMessage.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/server/ServerConnectHandler.cpp ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/coms/ComsAddPlayerMessage.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/common/ARGParser.h ../src/scorched/ServerDialog.h ../src/ode/odecpp.h ../src/server/ServerConnectHandler.h ../src/tank/TankScore.h
../src/scorched/Release/srcGLEXTGLStatescorched.o: ../src/GLEXT/GLState.cpp ../src/common/Defines.h ../src/GLEXT/GLState.h
../src/scorched/Release/srcactionsSpriteProjectilescorched.o: ../src/actions/SpriteProjectile.h ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/ode/contact.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/ode/memory.h ../src/engine/Action.h ../src/actions/SpriteProjectile.cpp ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/ode/collision.h ../src/ode/space.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/coms/NetBuffer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/ode/config.h ../src/ode/odecpp.h
../src/scorched/Release/srccomsComsActionsMessagescorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/ode/contact.h ../src/ode/rotation.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/coms/ComsActionsMessage.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/ode/collision.h ../src/ode/space.h ../src/coms/ComsMessage.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/ode/config.h ../src/ode/odecpp.h ../src/coms/ComsActionsMessage.cpp ../src/engine/ActionController.h
../src/scorched/Release/srccommonVectorscorched.o: ../src/common/Vector.h ../src/common/Defines.h ../src/common/Vector.cpp
../src/scorched/Release/srccommonWindowManagerscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/GLEXT/GLTexture.h ../src/dialogs/MainMenuDialog.h ../src/GLEXT/GLMenuI.h ../src/common/Keyboard.h ../src/engine/GameStateI.h ../src/GLEXT/GLMenuEntry.h ../src/common/Defines.h ../src/GLW/GLWVisiblePanel.h ../src/common/WindowManager.cpp ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/GLEXT/GLImage.h ../src/common/WindowManager.h ../src/GLEXT/GLFont2d.h ../src/GLEXT/GLMenu.h ../src/GLW/GLWWindow.h
../src/scorched/Release/srcscorchedSingleDialogscorched.o: ../src/scorched/MainDialog.h ../src/common/FileLines.h ../src/common/Defines.h ../src/scorched/SettingsDialog.h ../src/common/OptionEntry.h ../src/common/OptionsParam.h ../src/scorched//SingleDialog.cpp ../src/common/OptionsGame.h ../src/coms/NetBuffer.h ../src/common/ARGParser.h
../src/scorched/Release/srcclientClientStatescorched.o: ../src/landscape/TriNodePool.h ../src/client/ClientState.cpp ../src/landscape/Wall.h ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/client/ShotTimer.h ../src/tank/TankPhysics.h ../src/engine/GameStateStimulusI.h ../src/GLEXT/GLCamera.h ../src/landscape/PatchGrid.h ../src/GLW/GLWidget.h ../src/GLEXT/GLFontBanner.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/client/ClientNextRoundState.h ../src/GLEXT/GLTexture.h ../src/tank/TankShields.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/landscape/WaterMapSurround.h ../src/tank/TankAutoDefense.h ../src/common/Line.h ../src/ode/memory.h ../src/engine/Action.h ../src/GLEXT/GLMenuI.h ../src/GLEXT/GLConsoleRuleFn.h ../src/landscape/Landscape.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/tankai/TankAIHumanCtrl.h ../src/ode/odecpp_collision.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/engine/FrameTimer.h ../src/client/Main2DCamera.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/client/MainCamera.h ../src/tankgraph/TracerStore.h ../src/landscape/SurroundDefs.h ../src/ode/collision_space.h ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/landscape/Patch.h ../src/tank/Tank.h ../src/client/ClientShotState.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/landscape/TriNode.h ../src/GLEXT/GLConsoleRuleMethod.h ../src/landscape/HeightMapSurround.h ../src/GLW/GLWPanel.h ../src/ode/collision.h ../src/ode/space.h ../src/tankgraph/TankRenderer.h ../src/GLW/GLWVisibleWidget.h ../src/weapons/Weapon.h ../src/landscape/WaterMapPoints.h ../src/weapons/Accessory.h ../src/GLEXT/GLConsole.h ../src/landscape/Smoke.h ../src/common/Timer.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/landscape/ShadowMap.h ../src/engine/PhysicsEngine.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLCameraFrustum.h ../src/weapons/Shield.h ../src/client/SpeedChange.h ../src/GLEXT/GLImage.h ../src/tank/TankModelId.h ../src/client/MainBanner.h ../src/common/WindowManager.h ../src/tank/TankState.h ../src/GLEXT/GLConsoleMethods.h ../src/common/ProgressCounter.h ../src/client/ClientState.h ../src/landscape/PatchSide.h ../src/ode/geom.h ../src/ode/common.h ../src/GLEXT/GLConsoleRule.h ../src/ode/objects.h ../src/ode/error.h ../src/GLEXT/GLFont2d.h ../src/GLEXT/GLConsoleLines.h ../src/GLEXT/GLTextureCubeMap.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/landscape/WaterMap.h ../src/GLW/GLWWindow.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/ode/odecpp.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h ../src/GLEXT/GLConsoleRules.h ../src/engine/MainLoop.h ../src/tankgraph/TankMenus.h ../src/tank/TankScore.h ../src/engine/ActionController.h
../src/scorched/Release/srctankaiTankAIComputerscorched.o: ../src/tankai/TankAIComputer.cpp ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/coms/ComsDefenseMessage.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/server/ServerShotHolder.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/tankai/TankAIStrings.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/actions/TankSay.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/coms/ComsPlayedMoveMessage.h ../src/tankai/TankAIComputer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tankai/TankAIComputerBuyer.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/engine/ActionController.h ../src/tank/TankScore.h
../src/scorched/Release/srcodegeomscorched.o: ../src/ode/contact.h ../src/ode/rotation.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/misc.h ../src/ode/mass.h ../src/ode/geom.cpp ../src/ode/space.h ../src/ode/matrix.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/ode/config.h
../src/scorched/Release/srclandscapeSunscorched.o: ../src/common/Vector.h ../src/common/FileLines.h ../src/GLEXT/GLTexture.h ../src/engine/GameStateI.h ../src/landscape/Sun.h ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/landscape/Sun.cpp ../src/engine/MainLoopI.h ../src/common/KeyboardHistory.h ../src/common/OptionsGame.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLCameraFrustum.h ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/common/ARGParser.h ../src/GLEXT/GLBitmap.h ../src/GLEXT/GLLenseFlare.h
../src/scorched/Release/srcserverServerPlayedMoveHandlerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/server/ServerShotHolder.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/server/ServerState.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/server/ServerPlayedMoveHandler.cpp ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/coms/ComsPlayedMoveMessage.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/server/ServerPlayedMoveHandler.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcGLEXTGLTextureCubeMapscorched.o: ../src/GLEXT/GLTextureCubeMap.cpp ../src/GLEXT/GLTexture.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLTextureCubeMap.h
../src/scorched/Release/srccommonFileListscorched.o: ../src/common/FileList.h ../src/common/FileList.cpp
../src/scorched/Release/srccommonDefinesscorched.o: ../src/common/Defines.cpp
../src/scorched/Release/srcserverServerWaitingForPlayersStatescorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/engine/GameStateStimulusI.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/server/ServerWaitingForPlayersState.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/server/ServerWaitingForPlayersState.cpp ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/common/ARGParser.h ../src/scorched/ServerDialog.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcGLWGLWProgressscorched.o: ../src/GLW/GLWidget.h ../src/GLW/GLWProgress.h ../src/GLW/GLWProgress.cpp ../src/common/Defines.h ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLState.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h
../src/scorched/Release/srczlibinfutilscorched.o: ../src/zlib/infutil.c
../src/scorched/Release/srcserverServerShotStatescorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/engine/GameStateStimulusI.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/server/ServerShotState.cpp ../src/server/ServerShotHolder.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/server/ServerState.h ../src/ode/memory.h ../src/engine/Action.h ../src/coms/ComsActionsMessage.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/tank/TankContainer.h ../src/coms/ComsPlayedMoveMessage.h ../src/ode/geom.h ../src/coms/ComsLevelMessage.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankStart.h ../src/tank/TankParachutes.h ../src/server/ServerShotState.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/landscape/MovementMap.h ../src/engine/ActionController.h ../src/tank/TankScore.h
../src/scorched/Release/srcodefastltsolvescorched.o: ../src/ode/fastltsolve.c
../src/scorched/Release/src3dsparseASEModelscorched.o: ../src/common/Vector.h ../src/common/Face.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/3dsparse/ASEModel.h ../src/3dsparse/ASEModel.cpp ../src/3dsparse/MeshLOD.h
../src/scorched/Release/srclandscapeLanscapescorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/landscape/PatchGrid.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/common/FileLines.h ../src/dialogs/WindDialog.h ../src/GLEXT/GLTexture.h ../src/landscape/WaterMapSurround.h ../src/landscape/DeformLandscape.h ../src/landscape/Landscape.h ../src/engine/GameStateI.h ../src/landscape/WaterMapModifier.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/GLEXT/GLBitmapModifier.h ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/landscape/Lanscape.cpp ../src/landscape/SurroundDefs.h ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLVertexArray.h ../src/common/OptionEntry.h ../src/GLEXT/GLStateExtension.h ../src/landscape/InfoMap.h ../src/GLEXT/GLState.h ../src/landscape/Patch.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/landscape/TriNode.h ../src/GLW/GLWPanel.h ../src/landscape/HeightMapSurround.h ../src/GLW/GLWVisibleWidget.h ../src/coms/ComsMessage.h ../src/landscape/WaterMapPoints.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/landscape/ShadowMap.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/landscape/HeightMapRenderer.h ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/landscape/PatchSide.h ../src/coms/ComsLevelMessage.h ../src/common/Resources.h ../src/GLEXT/GLTextureCubeMap.h ../src/GLW/GLWWindow.h ../src/landscape/WaterMap.h ../src/common/OptionsTransient.h ../src/common/ARGParser.h ../src/common/ResourceFile.h ../src/landscape/MovementMap.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcGLEXTGLLuminancescorched.o: ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLEXT/GLLuminance.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLLuminance.cpp
../src/scorched/Release/srcodestepscorched.o: ../src/ode/rotation.h ../src/ode/odemath.h ../src/ode/timer.h ../src/ode/matrix.h ../src/ode/step.cpp ../src/ode/common.h ../src/ode/error.h ../src/ode/config.h
../src/scorched/Release/srczlibtreesscorched.o: ../src/zlib/trees.c
../src/scorched/Release/srcGLWGLWLabelscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/GLEXT/GLTexture.h ../src/GLW/GLWLabel.cpp ../src/common/Defines.h ../src/GLW/GLWLabel.h ../src/GLEXT/GLState.h ../src/GLW/GLWVisibleWidget.h ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLFont2d.h
../src/scorched/Release/srcengineMainLoopscorched.o: ../src/common/Vector.h ../src/common/FileLines.h ../src/engine/MainLoop.cpp ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/common/Timer.h ../src/coms/NetBuffer.h ../src/common/ARGParser.h ../src/engine/MainLoop.h
../src/scorched/Release/srclandscapeInfoMapscorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/common/Vector.h ../src/landscape/PatchGrid.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/GLEXT/GLTexture.h ../src/GLEXT/GLConsoleRuleMethodIAdapter.h ../src/landscape/WaterMapSurround.h ../src/GLEXT/GLConsoleRuleFn.h ../src/landscape/Landscape.h ../src/engine/GameStateI.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/landscape/SurroundDefs.h ../src/landscape/InfoMap.h ../src/GLEXT/GLState.h ../src/landscape/Patch.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/landscape/TriNode.h ../src/GLEXT/GLConsoleRuleMethod.h ../src/landscape/HeightMapSurround.h ../src/coms/ComsMessage.h ../src/landscape/WaterMapPoints.h ../src/GLEXT/GLConsole.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/landscape/ShadowMap.h ../src/landscape/InfoMap.cpp ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLConsoleMethods.h ../src/common/ProgressCounter.h ../src/landscape/PatchSide.h ../src/coms/ComsLevelMessage.h ../src/GLEXT/GLConsoleRule.h ../src/GLEXT/GLFont2d.h ../src/GLEXT/GLConsoleLines.h ../src/GLEXT/GLTextureCubeMap.h ../src/landscape/WaterMap.h ../src/common/OptionsTransient.h ../src/landscape/MovementMap.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h ../src/GLEXT/GLConsoleRules.h
../src/scorched/Release/srclandscapeHemispherescorched.o: ../src/common/Vector.h ../src/landscape/Hemisphere.h ../src/common/Defines.h ../src/landscape/Hemisphere.cpp ../src/GLEXT/GLState.h
../src/scorched/Release/srcengineActionControllerscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/common/FileLines.h ../src/ode/contact.h ../src/ode/rotation.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/engine/MainLoopI.h ../src/ode/collision.h ../src/ode/space.h ../src/common/OptionsParam.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/engine/ActionController.cpp ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/engine/ActionController.h
../src/scorched/Release/src3dsparseASEArrayFactscorched.o: ../src/common/Vector.h ../src/common/Face.h ../src/GLEXT/GLVertexTexArray.h ../src/common/Defines.h ../src/GLEXT/GLVertexArray.h ../src/GLEXT/GLState.h ../src/3dsparse/ASEModel.h ../src/3dsparse/ASEArrayFact.h ../src/3dsparse/ASEArrayFact.cpp
../src/scorched/Release/srcGLEXTGLConsoleFileReaderscorched.o: ../src/common/Vector.h ../src/XML/XMLFile.h ../src/common/FileLines.h ../src/GLEXT/GLTexture.h ../src/XML/XMLParser.h ../src/GLEXT/GLConsoleRuleFn.h ../src/engine/GameStateI.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/GLEXT/GLConsoleRuleMethod.h ../src/GLEXT/GLConsole.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLConsoleFileReader.h ../src/GLEXT/GLConsoleMethods.h ../src/GLEXT/GLConsoleRule.h ../src/GLEXT/GLFont2d.h ../src/GLEXT/GLConsoleLines.h ../src/XML/expat.h ../src/GLEXT/GLConsoleRules.h ../src/GLEXT/GLConsoleFileReader.cpp
../src/scorched/Release/srctankaiTankAIHumanCtrlscorched.o: ../src/tankai/TankAIHumanCtrl.cpp ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/tankai/TankAIHumanCtrl.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/client/ClientState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcactionsTankDamagescorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/actions/TankFalling.h ../src/tank/TankPhysics.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/tank/TankShields.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/actions/TankDead.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/actions/TankDamage.cpp ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/actions/TankDamage.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/coms/ComsLevelMessage.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/landscape/MovementMap.h ../src/engine/ActionController.h ../src/tank/TankScore.h
../src/scorched/Release/srcdialogsTalkDialogscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/GLW/GLWButton.h ../src/GLEXT/GLMenuI.h ../src/engine/GameStateI.h ../src/dialogs/TalkDialog.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/GLW/GLWVisiblePanel.h ../src/GLW/GLWLabel.h ../src/GLEXT/GLState.h ../src/dialogs/TalkDialog.cpp ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/coms/ComsMessage.h ../src/common/KeyboardHistory.h ../src/coms/ComsTextMessage.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/common/WindowManager.h ../src/GLW/GLWTextBox.h ../src/GLW/GLWWindow.h ../src/GLW/GLWTextButton.h
../src/scorched/Release/srcGLEXTGLConsoleRuleFnIAdapterscorched.o: ../src/common/Vector.h ../src/common/FileLines.h ../src/GLEXT/GLTexture.h ../src/GLEXT/GLConsoleRuleFn.h ../src/engine/GameStateI.h ../src/common/Defines.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/GLEXT/GLConsoleRuleFnIAdapter.cpp ../src/engine/MainLoopI.h ../src/GLEXT/GLConsoleRuleMethod.h ../src/GLEXT/GLConsole.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLConsoleMethods.h ../src/GLEXT/GLConsoleRuleFnIAdapter.h ../src/GLEXT/GLConsoleRule.h ../src/GLEXT/GLFont2d.h ../src/GLEXT/GLConsoleLines.h ../src/common/ARGParser.h ../src/GLEXT/GLConsoleRules.h
../src/scorched/Release/srclandscapePatchGridscorched.o: ../src/landscape/TriNodePool.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/GLEXT/GLCamera.h ../src/landscape/PatchGrid.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/common/Line.h ../src/GLEXT/GLMenuI.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/landscape/PatchGrid.cpp ../src/ode/odecpp_collision.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/client/MainCamera.h ../src/ode/collision_space.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/landscape/Patch.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/landscape/TriNode.h ../src/GLEXT/GLVar.h ../src/landscape/PatchVar.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/GLEXT/GLCameraFrustum.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/tank/TankContainer.h ../src/landscape/PatchSide.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/landscape/Variance.h ../src/tank/TankScore.h
../src/scorched/Release/srccommonRegistryscorched.o: ../src/common/Registry.cpp ../src/common/Registry.h
../src/scorched/Release/srcspritesExplosionRendererscorched.o: ../src/actions/SpriteProjectile.h ../src/landscape/TriNodePool.h ../src/engine/ActionMeta.h ../src/landscape/Wall.h ../src/common/Vector.h ../src/sprites/SmokeActionRenderer.h ../src/tank/TankPhysics.h ../src/landscape/PatchGrid.h ../src/ode/contact.h ../src/common/FileLines.h ../src/tank/TankWeapon.h ../src/tank/TankShields.h ../src/ode/rotation.h ../src/GLEXT/GLTexture.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/landscape/WaterMapSurround.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/landscape/Landscape.h ../src/sprites/DebrisActionRenderer.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/tank/TankController.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/ode/mass.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/ode/collision_space.h ../src/landscape/SurroundDefs.h ../src/common/Sound.h ../src/GLEXT/GLVertexArray.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/landscape/Patch.h ../src/tank/Tank.h ../src/sprites/ExplosionRenderer.cpp ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/landscape/TriNode.h ../src/ode/collision.h ../src/ode/space.h ../src/landscape/HeightMapSurround.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/landscape/WaterMapPoints.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/sprites/ExplosionTextures.h ../src/landscape/ShadowMap.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/GLEXT/GLCameraFrustum.h ../src/tank/TankModelId.h ../src/GLEXT/GLImage.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/common/SoundStore.h ../src/ode/geom.h ../src/landscape/PatchSide.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/sprites/ExplosionRenderer.h ../src/engine/PhysicsParticleObject.h ../src/GLEXT/GLTextureCubeMap.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/common/SoundBuffer.h ../src/landscape/WaterMap.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/GLEXT/GLTextureSet.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h ../src/tank/TankScore.h ../src/engine/ActionController.h
../src/scorched/Release/srcclientGLSetupscorched.o: ../src/client/GLSetup.h ../src/client/GLSetup.cpp ../src/common/Defines.h ../src/GLEXT/GLStateExtension.h ../src/GLEXT/GLState.h
../src/scorched/Release/srcGLEXTGLFontBannerscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/GLEXT/GLFontBanner.h ../src/GLEXT/GLTexture.h ../src/GLEXT/GLConsoleRuleFn.h ../src/engine/GameStateI.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/GLEXT/GLConsoleRuleMethod.h ../src/GLEXT/GLConsole.h ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLConsoleMethods.h ../src/GLEXT/GLFontBanner.cpp ../src/GLEXT/GLConsoleRule.h ../src/GLEXT/GLFont2d.h ../src/GLEXT/GLConsoleLines.h ../src/GLEXT/GLConsoleRules.h
../src/scorched/Release/srcweaponsAccessoryscorched.o: ../src/weapons/Accessory.h ../src/weapons/Accessory.cpp
../src/scorched/Release/srctankaiTankAIComputerMoronscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tankai/TankAIComputerMoron.cpp ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/tankai/TankAIComputerMoron.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/weapons/AccessoryStore.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tankai/TankAIComputer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tankai/TankAIComputerBuyer.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srczlibgzioscorched.o: ../src/zlib/gzio.c
../src/scorched/Release/srcodematrixscorched.o: ../src/ode/matrix.h ../src/ode/common.h ../src/ode/error.h ../src/ode/matrix.cpp ../src/ode/config.h
../src/scorched/Release/srcengineScorchedCollisionIdsscorched.o: ../src/engine/ScorchedCollisionIds.h ../src/engine/ScorchedCollisionIds.cpp
../src/scorched/Release/srclandscapeSmokeChainscorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/common/Vector.h ../src/landscape/PatchGrid.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/GLEXT/GLTexture.h ../src/landscape/WaterMapSurround.h ../src/landscape/Landscape.h ../src/engine/GameStateI.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/landscape/Surround.h ../src/landscape/SmokeChain.h ../src/landscape/SurroundDefs.h ../src/GLEXT/GLState.h ../src/landscape/Patch.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/landscape/TriNode.h ../src/landscape/HeightMapSurround.h ../src/landscape/SmokeChain.cpp ../src/landscape/WaterMapPoints.h ../src/coms/ComsMessage.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/landscape/ShadowMap.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/landscape/PatchSide.h ../src/coms/ComsLevelMessage.h ../src/GLEXT/GLTextureCubeMap.h ../src/landscape/WaterMap.h ../src/common/OptionsTransient.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h ../src/landscape/MovementMap.h
../src/scorched/Release/srccommonOptionsTransientscorched.o: ../src/common/Vector.h ../src/common/FileLines.h ../src/common/OptionsTransient.cpp ../src/common/Defines.h ../src/common/OptionEntry.h ../src/common/OptionsGame.h ../src/coms/NetBuffer.h ../src/common/OptionsTransient.h ../src/common/ARGParser.h
../src/scorched/Release/srcserverServerStatescorched.o: ../src/common/Vector.h ../src/engine/GameStateStimulusI.h ../src/server/ServerTooFewPlayersStimulus.h ../src/server/ServerWaitingForPlayersState.h ../src/server/ServerScoreState.h ../src/server/ServerState.h ../src/tankai/TankAIComputerCtrl.h ../src/engine/GameStateI.h ../src/server/ServerState.cpp ../src/server/ServerNextRoundState.h ../src/engine/MainLoopI.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/server/ServerPlayingState.h ../src/server/ServerShotState.h ../src/server/ServerNewGameState.h
../src/scorched/Release/srccommonOptionsGamescorched.o: ../src/common/Vector.h ../src/common/FileLines.h ../src/common/Defines.h ../src/common/OptionEntry.h ../src/common/OptionsGame.h ../src/coms/NetBuffer.h ../src/common/OptionsGame.cpp ../src/common/OptionsTransient.h ../src/common/ARGParser.h
../src/scorched/Release/srcGLWGLWVisiblePanelscorched.o: ../src/GLW/GLWidget.h ../src/GLW/GLWVisiblePanel.cpp ../src/common/Defines.h ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLState.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h
../src/scorched/Release/srcXMLxmlrolescorched.o: ../src/XML/expat_config.h ../src/XML/xmlrole.c
../src/scorched/Release/srcodematscorched.o: ../src/ode/memory.h ../src/ode/misc.h ../src/ode/matrix.h ../src/ode/mat.cpp ../src/ode/common.h ../src/ode/error.h ../src/ode/config.h
../src/scorched/Release/srcGLWGLWScrollWscorched.o: ../src/GLW/GLWidget.h ../src/GLW/GLWButton.h ../src/GLW/GLWScrollW.h ../src/GLW/GLWScrollButton.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h ../src/GLW/GLWScrollW.cpp
../src/scorched/Release/srctankaiTankAIStorescorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/tankai/TankAIComputerMoron.h ../src/tankai/TankAIComputerDHTest.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/tankai/TankAIComputerChooser.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tankai/TankAIStore.h ../src/tank/TankBatteries.h ../src/tankai/TankAIComputerShooter.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tankai/TankAIStore.cpp ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tankai/TankAIComputerTosser.h ../src/tankai/TankAIComputer.h ../src/tankai/TankAIComputerRandom.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tankai/TankAIComputerBuyer.h ../src/tankai/TankAIComputerPShark.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srczlibcompressscorched.o: ../src/zlib/compress.c
../src/scorched/Release/srcdialogsConnectDialogscorched.o: ../src/common/Vector.h ../src/client/ClientNewGameHandler.h ../src/dialogs/PlayerDialog.h ../src/GLW/GLWidget.h ../src/client/ClientStartGameHandler.h ../src/GLEXT/GLFontBanner.h ../src/common/FileLines.h ../src/coms/ComsMessageHandler.h ../src/client/ClientRmPlayerHandler.h ../src/GLW/GLWButton.h ../src/GLEXT/GLMenuI.h ../src/client/ClientDefenseHandler.h ../src/client/ClientScoreHandler.h ../src/engine/GameStateI.h ../src/GLW/GLWScrollW.h ../src/dialogs/ConnectDialog.h ../src/GLW/GLWScrollButton.h ../src/client/ClientTextHandler.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/common/OptionsDisplay.h ../src/client/ClientActionsHandler.h ../src/GLW/GLWVisiblePanel.h ../src/coms/ComsConnectMessage.h ../src/common/OptionEntry.h ../src/GLEXT/GLVertexArray.h ../src/GLW/GLWLabel.h ../src/dialogs/QuitDialog.h ../src/coms/NetClient.h ../src/client/ClientBuyAccessoryHandler.h ../src/GLEXT/GLState.h ../src/dialogs/ConnectDialog.cpp ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/client/ClientMessageHandler.h ../src/GLW/GLWPanel.h ../src/common/OptionsParam.h ../src/GLW/GLWVisibleWidget.h ../src/coms/ComsMessage.h ../src/common/Timer.h ../src/tankgraph/GLWTankViewer.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/client/ClientGameStoppedHandler.h ../src/client/MainBanner.h ../src/common/WindowManager.h ../src/client/ClientConnectionAcceptHandler.h ../src/coms/NetMessageHandler.h ../src/GLW/GLWTextBox.h ../src/common/Logger.h ../src/GLW/GLWListView.h ../src/GLW/GLWWindow.h ../src/common/ARGParser.h ../src/client/ClientAddPlayerHandler.h ../src/engine/MainLoop.h ../src/GLW/GLWTextButton.h
../src/scorched/Release/srcGLEXTGLVarscorched.o: ../src/GLEXT/GLVar.cpp ../src/common/Defines.h ../src/GLEXT/GLStateExtension.h ../src/GLEXT/GLState.h ../src/GLEXT/GLVar.h
../src/scorched/Release/srcclientClientMessageHandlerscorched.o: ../src/common/Vector.h ../src/coms/ComsMessageHandler.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/client/ClientMessageHandler.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/coms/NetMessageHandler.h ../src/client/ClientMessageHandler.cpp ../src/client/ClientState.h ../src/common/Logger.h
../src/scorched/Release/srcGLEXTGLBitmapModifierscorched.o: ../src/common/Vector.h ../src/landscape/DeformLandscape.h ../src/landscape/HeightMap.h ../src/GLEXT/GLBitmapModifier.h ../src/common/Defines.h ../src/GLEXT/GLBitmapItterator.h ../src/GLEXT/GLState.h ../src/GLEXT/GLBitmapModifier.cpp ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcclientClientStartGameHandlerscorched.o: ../src/landscape/TriNodePool.h ../src/engine/ActionMeta.h ../src/landscape/Wall.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/client/ClientNewGameHandler.h ../src/landscape/PatchGrid.h ../src/common/FileLines.h ../src/ode/contact.h ../src/client/ClientStartGameHandler.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/GLEXT/GLTexture.h ../src/engine/ScorchedCollisionIds.h ../src/landscape/WaterMapSurround.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/landscape/Landscape.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/ode/collision_space.h ../src/landscape/SurroundDefs.h ../src/common/Sound.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/landscape/Patch.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/landscape/TriNode.h ../src/ode/collision.h ../src/ode/space.h ../src/landscape/HeightMapSurround.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/landscape/WaterMapPoints.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/landscape/ShadowMap.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/GLEXT/GLImage.h ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/common/SoundStore.h ../src/tank/TankContainer.h ../src/client/ClientState.h ../src/client/ClientStartGameHandler.cpp ../src/ode/geom.h ../src/landscape/PatchSide.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/GLEXT/GLTextureCubeMap.h ../src/tank/TankFuel.h ../src/tank/TankStart.h ../src/common/SoundBuffer.h ../src/tank/TankParachutes.h ../src/landscape/WaterMap.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/common/ARGParser.h ../src/coms/ComsStartGameMessage.h ../src/ode/odecpp.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h ../src/coms/ComsPlayerStateMessage.h ../src/engine/ActionController.h ../src/tank/TankScore.h
../src/scorched/Release/srczlibdeflatescorched.o: ../src/zlib/deflate.c
../src/scorched/Release/srccommonGammascorched.o: ../src/common/Gamma.cpp ../src/common/Defines.h ../src/common/Gamma.h
../src/scorched/Release/srccomsComsNewGameMessagescorched.o: ../src/coms/ComsNewGameMessage.cpp ../src/coms/NetMessage.h ../src/coms/ComsMessage.h ../src/coms/NetBuffer.h ../src/coms/ComsLevelMessage.h ../src/coms/ComsNewGameMessage.h
../src/scorched/Release/srcenginePhysicsEnginescorched.o: ../src/common/Vector.h ../src/ode/contact.h ../src/ode/rotation.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/ode/collision.h ../src/ode/space.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsEngine.cpp ../src/ode/config.h ../src/ode/odecpp.h
../src/scorched/Release/srcengineGameStatescorched.o: ../src/common/Vector.h ../src/engine/GameStateStimulusI.h ../src/common/Keyboard.h ../src/engine/GameStateI.h ../src/common/Defines.h ../src/engine/MainLoopI.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/engine/GameState.cpp
../src/scorched/Release/srccommonLinescorched.o: ../src/common/Vector.h ../src/common/Line.h ../src/common/Line.cpp
../src/scorched/Release/srcGLEXTGLConsoleRulesscorched.o: ../src/GLEXT/GLConsoleRules.cpp ../src/common/Defines.h ../src/GLEXT/GLConsoleRule.h ../src/GLEXT/GLConsoleRules.h
../src/scorched/Release/srcodefastldltscorched.o: ../src/ode/fastldlt.c
../src/scorched/Release/srccomsNetMessagePoolscorched.o: ../src/coms/NetMessage.h ../src/coms/NetBuffer.h ../src/coms/NetMessagePool.cpp ../src/coms/NetMessagePool.h
../src/scorched/Release/srclandscapeHeightMapLoaderscorched.o: ../src/landscape/HeightMapLoader.h ../src/common/Vector.h ../src/common/RandomGenerator.h ../src/landscape/HeightMapLoader.cpp ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/landscape/HeightMapModifier.h ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcodeodemathscorched.o: ../src/ode/odemath.h ../src/ode/common.h ../src/ode/error.h ../src/ode/config.h ../src/ode/odemath.cpp
../src/scorched/Release/srclandscapeTriNodePoolscorched.o: ../src/landscape/TriNodePool.h ../src/landscape/TriNode.h ../src/landscape/TriNodePool.cpp
../src/scorched/Release/srclandscapeWaterMapModifierscorched.o: ../src/common/Vector.h ../src/GLEXT/GLTexture.h ../src/landscape/WaterMapSurround.h ../src/landscape/WaterMapModifier.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/landscape/WaterMapModifier.cpp ../src/GLEXT/GLState.h ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/GLEXT/GLTextureCubeMap.h ../src/landscape/WaterMap.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srccomsNetMessageHandlerscorched.o: ../src/coms/NetMessageHandler.cpp ../src/coms/NetMessage.h ../src/coms/NetBuffer.h ../src/coms/NetMessageHandler.h ../src/coms/NetMessagePool.h
../src/scorched/Release/src3dsparseASEFilescorched.o: ../src/common/Vector.h ../src/3dsparse/ASEFile.h ../src/common/Face.h ../src/common/Defines.h ../src/3dsparse/ASEModel.h ../src/3dsparse/ASEFile.cpp
../src/scorched/Release/srctankTankContainerscorched.o: ../src/tank/TankContainer.cpp ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srccommonSoundStorescorched.o: ../src/common/SoundStore.cpp ../src/common/Defines.h ../src/common/Sound.h ../src/common/SoundStore.h ../src/common/SoundBuffer.h
../src/scorched/Release/srcweaponsWeaponNapalmscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/PhysicsParticle.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/actions/ShotProjectileNapalm.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/weapons/WeaponNapalm.cpp ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/weapons/WeaponNapalm.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcengineGameStateStimulusIscorched.o: ../src/engine/GameStateStimulusI.h ../src/engine/GameStateStimulusI.cpp
../src/scorched/Release/src3dsparseMeshLODTriscorched.o: ../src/common/Vector.h ../src/common/Defines.h ../src/3dsparse/MeshLODTri.cpp ../src/3dsparse/MeshLODTri.h ../src/3dsparse/MeshLODVector.h
../src/scorched/Release/srcdialogsTankDialogscorched.o: ../src/common/Vector.h ../src/dialogs/TankDialog.cpp ../src/tank/TankPhysics.h ../src/GLEXT/GLCamera.h ../src/GLW/GLWidget.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/GLEXT/GLTexture.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/common/Line.h ../src/GLEXT/GLMenuI.h ../src/tankgraph/TankModelRenderer.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/client/MainCamera.h ../src/ode/collision_space.h ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLVertexArray.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/GLEXT/GLImage.h ../src/tank/TankModelId.h ../src/tankgraph/TankMesh.h ../src/tank/TankState.h ../src/tankgraph/TankModel.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/dialogs/TankDialog.h ../src/ode/objects.h ../src/ode/error.h ../src/GLEXT/GLFont2d.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/GLW/GLWWindow.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcodeerrorscorched.o: ../src/ode/error.cpp ../src/ode/error.h ../src/ode/config.h
../src/scorched/Release/srcscorchedNetDialogscorched.o: ../src/scorched/MainDialog.h ../src/scorched/NetDialog.h ../src/common/FileLines.h ../src/common/Defines.h ../src/common/OptionEntry.h ../src/common/OptionsParam.h ../src/coms/NetBuffer.h ../src/scorched//NetDialog.cpp ../src/common/ARGParser.h
../src/scorched/Release/srccomsComsDefenseMessagescorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/coms/ComsDefenseMessage.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/coms/ComsDefenseMessage.cpp ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcenginePhysicsParticleObjectscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/ode/contact.h ../src/ode/rotation.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/engine/MainLoopI.h ../src/engine/PhysicsParticleObject.cpp ../src/ode/collision.h ../src/ode/space.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/ode/config.h ../src/ode/odecpp.h ../src/engine/ActionController.h
../src/scorched/Release/srctankgraphTankRendererscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/GLEXT/GLTexture.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/GLEXT/GLMenuI.h ../src/tankgraph/TankModelRenderer.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/tankgraph/TracerStore.h ../src/ode/collision_space.h ../src/GLEXT/GLVertexArray.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/tankgraph/TankRenderer.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/GLEXT/GLImage.h ../src/tank/TankModelId.h ../src/tankgraph/TankMesh.h ../src/tank/TankState.h ../src/client/ClientState.h ../src/tank/TankContainer.h ../src/tankgraph/TankModel.h ../src/ode/geom.h ../src/tankgraph/TankRenderer.cpp ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tankgraph/TankMenus.h ../src/tank/TankScore.h
../src/scorched/Release/srctankaiTankAIComputerBuyerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/tankai/TankAIComputerBuyer.cpp ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/weapons/AccessoryStore.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tankai/TankAIComputerBuyer.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srccommonTimerscorched.o: ../src/common/Timer.cpp ../src/common/Defines.h ../src/common/Timer.h
../src/scorched/Release/src3dsparseASEStorescorched.o: ../src/common/Vector.h ../src/3dsparse/ASEFile.h ../src/common/Face.h ../src/GLEXT/GLVertexTexArray.h ../src/common/Defines.h ../src/3dsparse/ASEStore.h ../src/GLEXT/GLVertexArray.h ../src/GLEXT/GLState.h ../src/3dsparse/ASEStore.cpp ../src/3dsparse/ASEModel.h ../src/3dsparse/ASEArrayFact.h
../src/scorched/Release/srcscorchedSettingsDialogscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/scorched/MainDialog.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tankai/TankAIStore.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/scorched/DisplayDialog.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tankai/TankAIComputer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tankai/TankAIComputerBuyer.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/scorched//SettingsDialog.cpp ../src/tank/TankScore.h
../src/scorched/Release/srclandscapeSurroundscorched.o: ../src/common/Vector.h ../src/GLEXT/GLCamera.h ../src/common/FileLines.h ../src/GLEXT/GLTexture.h ../src/common/Line.h ../src/GLEXT/GLMenuI.h ../src/landscape/Hemisphere.h ../src/engine/GameStateI.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/client/MainCamera.h ../src/landscape/Surround.h ../src/landscape/SurroundDefs.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/common/FileList.h ../src/landscape/Surround.cpp ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/common/Resources.h ../src/common/OptionsTransient.h ../src/common/ARGParser.h ../src/common/ResourceFile.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srclandscapeShadowMapscorched.o: ../src/GLEXT/GLTexture.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/landscape/ShadowMap.h ../src/GLEXT/GLImage.h ../src/landscape/ShadowMap.cpp
../src/scorched/Release/srclandscapeSurroundDefsscorched.o: ../src/common/Vector.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/landscape/SurroundDefs.h ../src/landscape/SurroundDefs.cpp ../src/common/ProgressCounter.h
../src/scorched/Release/srcXMLXMLParserscorched.o: ../src/XML/XMLParser.h ../src/common/Defines.h ../src/XML/XMLParser.cpp ../src/XML/expat.h
../src/scorched/Release/srctankaiTankAIAdderscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/tankai/TankAIStrings.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tankai/TankAIStore.h ../src/tank/TankBatteries.h ../src/tank/TankColorGenerator.h ../src/ode/collision.h ../src/ode/space.h ../src/common/OptionsParam.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/tankai/TankAIAdder.cpp ../src/ode/matrix.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/tankai/TankAIComputer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tankai/TankAIComputerBuyer.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/common/ARGParser.h ../src/tankai/TankAIAdder.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcserverServerMessageHandlerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/server/ServerMessageHandler.cpp ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/coms/ComsRmPlayerMessage.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/server/ServerMessageHandler.h ../src/scorched/ServerDialog.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srccommonRandomGeneratorscorched.o: ../src/common/RandomGenerator.h ../src/common/Defines.h ../src/common/RandomGenerator.cpp
../src/scorched/Release/srclandscapeHeightMapModifierscorched.o: ../src/common/Vector.h ../src/common/RandomGenerator.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/landscape/HeightMapModifier.h ../src/common/ProgressCounter.h ../src/landscape/HeightMapModifier.cpp
../src/scorched/Release/srcscorchedServerDialogscorched.o: ../src/common/Vector.h ../src/scorched//ServerDialog.cpp ../src/tank/TankPhysics.h ../src/scorched/MainDialog.h ../src/ode/contact.h ../src/common/FileLines.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/server/ServerState.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/server/ServerMain.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/ode/matrix.h ../src/coms/ComsTextMessage.h ../src/engine/PhysicsEngine.h ../src/common/OptionsGame.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetServer.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/common/ARGParser.h ../src/coms/NetBufferUtil.h ../src/scorched/ServerDialog.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcweaponsWeaponFunkyscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/PhysicsParticle.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/actions/ShotProjectileExplosion.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/weapons/WeaponFunky.cpp ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/WeaponFunky.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/actions/ShotProjectileFunky.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcengineActionscorched.o: ../src/engine/Action.h ../src/engine/Action.cpp
../src/scorched/Release/srcodelcpscorched.o: ../src/ode/misc.h ../src/ode/timer.h ../src/ode/matrix.h ../src/ode/lcp.cpp ../src/ode/common.h ../src/ode/error.h ../src/ode/config.h
../src/scorched/Release/srczlibinfblockscorched.o: ../src/zlib/infblock.c
../src/scorched/Release/srcengineGameStateIscorched.o: ../src/common/Vector.h ../src/engine/GameStateI.h ../src/engine/MainLoopI.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/engine/GameStateI.cpp
../src/scorched/Release/srctankgraphTankMeshscorched.o: ../src/common/Vector.h ../src/3dsparse/ASEFile.h ../src/common/Face.h ../src/GLEXT/GLVertexTexArray.h ../src/common/Defines.h ../src/GLEXT/GLVertexArray.h ../src/GLEXT/GLState.h ../src/3dsparse/ASEModel.h ../src/3dsparse/ASEArrayFact.h ../src/tankgraph/TankMesh.h ../src/tankgraph/TankMesh.cpp
../src/scorched/Release/srccomsComsAddPlayerMessagescorched.o: ../src/coms/ComsAddPlayerMessage.cpp ../src/common/Vector.h ../src/coms/NetMessage.h ../src/coms/ComsMessage.h ../src/coms/NetBuffer.h ../src/coms/ComsAddPlayerMessage.h
../src/scorched/Release/srclandscapePatchSidescorched.o: ../src/landscape/TriNodePool.h ../src/common/Vector.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/landscape/PatchSide.cpp ../src/landscape/TriNode.h ../src/GLEXT/GLVar.h ../src/landscape/PatchVar.h ../src/common/ProgressCounter.h ../src/landscape/PatchSide.h ../src/landscape/Variance.h
../src/scorched/Release/srccomsComsLevelMessagescorched.o: ../src/coms/ComsLevelMessage.cpp ../src/common/Defines.h ../src/coms/NetMessage.h ../src/coms/ComsMessage.h ../src/coms/NetBuffer.h ../src/coms/ComsLevelMessage.h
../src/scorched/Release/srcengineFrameTimerscorched.o: ../src/common/Vector.h ../src/engine/FrameTimer.cpp ../src/common/FileLines.h ../src/engine/GameStateI.h ../src/engine/FrameTimer.h ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/GLEXT/GLVar.h ../src/landscape/PatchVar.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/common/Logger.h ../src/common/ARGParser.h
../src/scorched/Release/srctankaiTankAIComputerShooterscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/tank/TankLib.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/tankai/TankAIComputerShooter.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/weapons/AccessoryStore.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tankai/TankAIComputerShooter.cpp ../src/tankai/TankAIComputer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tankai/TankAIComputerBuyer.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcGLEXTGLFont2dscorched.o: ../src/common/Vector.h ../src/GLEXT/GLTexture.h ../src/engine/GameStateI.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/GLEXT/GLFont2d.cpp ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/GLEXT/GLCameraFrustum.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLFont2d.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srccommonResourcesscorched.o: ../src/common/Resources.cpp ../src/common/Resources.h ../src/common/ResourceFile.h
../src/scorched/Release/srcactionsTankResignscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/actions/TankResign.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/actions/TankResign.cpp ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srctankTankFuelscorched.o: ../src/common/Defines.h ../src/tank/TankFuel.cpp ../src/coms/NetBuffer.h ../src/tank/TankFuel.h
../src/scorched/Release/srccomsComsMessagescorched.o: ../src/coms/NetMessage.h ../src/coms/ComsMessage.h ../src/coms/NetBuffer.h ../src/coms/ComsMessage.cpp
../src/scorched/Release/srcserverServerScoreStatescorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/engine/GameStateStimulusI.h ../src/tank/TankSort.h ../src/ode/contact.h ../src/common/FileLines.h ../src/coms/ComsScoreMessage.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/server/ServerScoreState.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/server/ServerScoreState.cpp ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/common/OptionsGame.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/common/ARGParser.h ../src/scorched/ServerDialog.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srctankgraphTankModelStorescorched.o: ../src/common/Vector.h ../src/3dsparse/ASEFile.h ../src/XML/XMLFile.h ../src/common/FileLines.h ../src/tankgraph/TankModelStore.cpp ../src/GLEXT/GLTexture.h ../src/XML/XMLParser.h ../src/common/Face.h ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/tankgraph/TankModelStore.h ../src/common/OptionEntry.h ../src/GLEXT/GLVertexArray.h ../src/GLEXT/GLState.h ../src/3dsparse/ASEModel.h ../src/coms/NetBuffer.h ../src/GLEXT/GLImage.h ../src/tank/TankModelId.h ../src/tankgraph/TankMesh.h ../src/tankgraph/TankModel.h ../src/common/ARGParser.h ../src/XML/expat.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcdialogsMainMenuDialogscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/GLEXT/GLTexture.h ../src/dialogs/MainMenuDialog.h ../src/GLEXT/GLMenuI.h ../src/GLEXT/GLMenuEntry.h ../src/common/Defines.h ../src/dialogs/MainMenuDialog.cpp ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLState.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLFont2d.h ../src/GLW/GLWWindow.h ../src/GLEXT/GLMenu.h
../src/scorched/Release/srcactionsShowScorescorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/actions/ShowScore.cpp ../src/engine/Action.h ../src/GLEXT/GLMenuI.h ../src/engine/GameStateI.h ../src/common/Defines.h ../src/actions/ShowScore.h ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/common/WindowManager.h ../src/GLW/GLWWindow.h ../src/dialogs/ScoreDialog.h
../src/scorched/Release/srccomsComsTextMessagescorched.o: ../src/common/Vector.h ../src/coms/ComsTextMessage.cpp ../src/coms/NetMessage.h ../src/coms/ComsMessage.h ../src/coms/ComsTextMessage.h ../src/coms/NetBuffer.h
../src/scorched/Release/srcspritesExplosionTexturesscorched.o: ../src/GLEXT/GLTexture.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/sprites/ExplosionTextures.h ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/sprites/ExplosionTextures.cpp ../src/GLEXT/GLTextureSet.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcodeodescorched.o: ../src/ode/contact.h ../src/ode/rotation.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/ode/ode.cpp ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/ode/collision.h ../src/ode/space.h ../src/ode/matrix.h ../src/ode/ode.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/ode/config.h ../src/ode/odecpp.h
../src/scorched/Release/srcGLWGLWPushButtonscorched.o: ../src/GLW/GLWidget.h ../src/GLW/GLWButton.h ../src/common/Defines.h ../src/GLW/GLWPushButton.cpp ../src/GLEXT/GLState.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h ../src/GLW/GLWPushButton.h
../src/scorched/Release/srctankTankAccessoriesscorched.o: ../src/common/Vector.h ../src/tank/TankWeapon.h ../src/tank/TankShields.h ../src/tank/TankAutoDefense.h ../src/common/Defines.h ../src/tank/TankAccessories.cpp ../src/tank/TankBatteries.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/weapons/AccessoryStore.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h
../src/scorched/Release/srcclientClientBuyAccessoryHandlerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/client/ClientBuyAccessoryHandler.cpp ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/Sound.h ../src/client/ClientBuyAccessoryHandler.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/common/SoundStore.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/coms/ComsBuyAccessoryMessage.h ../src/tank/TankFuel.h ../src/common/SoundBuffer.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcactionsShotProjectileNapalmscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/ode/contact.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/ode/memory.h ../src/engine/Action.h ../src/actions/ShotProjectileNapalm.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/actions/ShotProjectileNapalm.cpp ../src/ode/timer.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/common/ProgressCounter.h ../src/ode/geom.h ../src/ode/common.h ../src/coms/ComsLevelMessage.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/actions/Napalm.h ../src/ode/config.h ../src/ode/odecpp.h ../src/landscape/MovementMap.h ../src/engine/ActionController.h
../src/scorched/Release/srcclientMainBannerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/GLEXT/GLCamera.h ../src/GLW/GLWidget.h ../src/ode/contact.h ../src/GLEXT/GLFontBanner.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/common/Line.h ../src/GLEXT/GLMenuI.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/client/MainCamera.h ../src/ode/collision_space.h ../src/client/MainBanner.cpp ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/client/MainBanner.h ../src/tank/TankState.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcdialogsHelpDialogscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/common/FileLines.h ../src/GLW/GLWButton.h ../src/GLEXT/GLMenuI.h ../src/engine/GameStateI.h ../src/GLW/GLWScrollW.h ../src/dialogs/HelpDialog.h ../src/GLW/GLWScrollButton.h ../src/common/Defines.h ../src/GLW/GLWFileView.h ../src/GLW/GLWVisiblePanel.h ../src/GLW/GLWLabel.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/common/WindowManager.h ../src/GLW/GLWWindow.h ../src/GLW/GLWTextButton.h ../src/dialogs/HelpDialog.cpp
../src/scorched/Release/srcactionsTankScoredscorched.o: ../src/actions/TankScored.cpp ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/common/FileLines.h ../src/ode/contact.h ../src/sprites/TextActionRenderer.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/common/OptionsParam.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/actions/TankScored.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcGLEXTGLCamerascorched.o: ../src/common/Vector.h ../src/GLEXT/GLCamera.h ../src/common/Line.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLEXT/GLCamera.cpp
../src/scorched/Release/srclandscapeTriNodescorched.o: ../src/landscape/TriNode.cpp ../src/landscape/TriNode.h
../src/scorched/Release/srcclientMain2DCamerascorched.o: ../src/common/Vector.h ../src/GLEXT/GLCamera.h ../src/common/Line.h ../src/GLEXT/GLMenuI.h ../src/engine/GameStateI.h ../src/client/Main2DCamera.h ../src/common/Defines.h ../src/client/MainCamera.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/client/Main2DCamera.cpp ../src/common/KeyboardHistory.h ../src/engine/GameState.h
../src/scorched/Release/srctankTankSortscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankSort.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/tank/TankSort.cpp ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcdialogsAutoDefenseDialogscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/GLW/GLWidget.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/dialogs/AutoDefenseDialog.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/GLW/GLWFlag.h ../src/tank/TankAutoDefense.h ../src/GLW/GLWButton.h ../src/ode/memory.h ../src/GLEXT/GLMenuI.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/GLW/GLWDropDown.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/GLW/GLWVisiblePanel.h ../src/GLW/GLWLabel.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/GLW/GLWPushButton.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/common/WindowManager.h ../src/tank/TankState.h ../src/client/ClientState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/dialogs/AutoDefenseDialog.cpp ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/GLW/GLWWindow.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/ode/odecpp.h ../src/GLW/GLWTextButton.h ../src/tank/TankScore.h
../src/scorched/Release/srcGLWGLWVisibleWidgetscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/GLW/GLWVisibleWidget.cpp ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h
../src/scorched/Release/srcweaponsWeaponTracerscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/weapons/WeaponTracer.cpp ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/PhysicsParticle.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/weapons/WeaponTracer.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/actions/ShotProjectileTracer.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcactionsTankMovementscorched.o: ../src/engine/ActionMeta.h ../src/actions/TankFalling.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/ode/contact.h ../src/actions/TankMovement.cpp ../src/tank/TankWeapon.h ../src/tank/TankShields.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/actions/TankMovement.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/coms/ComsLevelMessage.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/landscape/MovementMap.h ../src/tank/TankScore.h ../src/engine/ActionController.h
../src/scorched/Release/srccommonOptionsDisplayscorched.o: ../src/common/Vector.h ../src/common/FileLines.h ../src/GLEXT/GLTexture.h ../src/GLEXT/GLConsoleRuleFn.h ../src/engine/GameStateI.h ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/GLEXT/GLConsoleRuleMethod.h ../src/common/OptionsDisplay.cpp ../src/GLEXT/GLConsole.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLConsoleMethods.h ../src/GLEXT/GLConsoleRuleFnIAdapter.h ../src/GLEXT/GLConsoleRule.h ../src/GLEXT/GLFont2d.h ../src/GLEXT/GLConsoleLines.h ../src/common/ARGParser.h ../src/GLEXT/GLConsoleRules.h
../src/scorched/Release/srcclientClientActionsHandlerscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/ode/contact.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/coms/ComsActionsMessage.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/ode/mass.h ../src/client/ClientActionsHandler.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/ode/collision.h ../src/ode/space.h ../src/client/ClientActionsHandler.cpp ../src/coms/ComsMessage.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/coms/NetMessageHandler.h ../src/client/ClientState.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/ode/config.h ../src/ode/odecpp.h ../src/engine/ActionController.h
../src/scorched/Release/srcscorchedDisplayDialogscorched.o: ../src/scorched//DisplayDialog.cpp ../src/scorched/MainDialog.h ../src/common/FileLines.h ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/common/OptionEntry.h ../src/scorched/DisplayDialog.h ../src/coms/NetBuffer.h ../src/common/ARGParser.h
../src/scorched/Release/srcGLEXTGLBitmapItteratorscorched.o: ../src/GLEXT/GLBitmapItterator.cpp ../src/common/Defines.h ../src/GLEXT/GLBitmapItterator.h ../src/GLEXT/GLState.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcGLWGLWTextButtonscorched.o: ../src/GLW/GLWidget.h ../src/GLW/GLWButton.h ../src/GLW/GLWTextButton.cpp ../src/common/Defines.h ../src/GLW/GLWLabel.h ../src/GLEXT/GLState.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h ../src/GLW/GLWTextButton.h
../src/scorched/Release/srcodetestingscorched.o: ../src/ode/memory.h ../src/ode/misc.h ../src/ode/testing.cpp ../src/ode/common.h ../src/ode/error.h ../src/ode/config.h
../src/scorched/Release/srcclientClientConnectionAcceptHandlerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/client/ClientConnectionAcceptHandler.cpp ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/coms/ComsConnectAcceptMessage.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/coms/NetMessageHandler.h ../src/client/ClientConnectionAcceptHandler.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcengineMainLoopIscorched.o: ../src/common/Vector.h ../src/engine/MainLoopI.cpp ../src/engine/MainLoopI.h
../src/scorched/Release/srccommonProgressCounterscorched.o: ../src/common/ProgressCounter.cpp ../src/common/ProgressCounter.h
../src/scorched/Release/srcdialogsKillDialogscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/GLW/GLWidget.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/GLW/GLWButton.h ../src/ode/memory.h ../src/GLEXT/GLMenuI.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/GLW/GLWVisiblePanel.h ../src/GLW/GLWLabel.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/dialogs/KillDialog.h ../src/ode/collision.h ../src/ode/space.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/Timer.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/dialogs/KillDialog.cpp ../src/common/WindowManager.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/GLW/GLWWindow.h ../src/ode/config.h ../src/ode/odecpp.h ../src/engine/MainLoop.h ../src/GLW/GLWTextButton.h ../src/tank/TankScore.h
../src/scorched/Release/srccommonDisplayscorched.o: ../src/common/Display.cpp ../src/common/Display.h
../src/scorched/Release/srcdialogsPlayerDialogscorched.o: ../src/common/Vector.h ../src/dialogs/PlayerDialog.h ../src/GLW/GLWidget.h ../src/common/FileLines.h ../src/GLW/GLWButton.h ../src/GLW/GLWScrollW.h ../src/GLW/GLWScrollButton.h ../src/common/Defines.h ../src/GLW/GLWVisiblePanel.h ../src/GLW/GLWLabel.h ../src/common/OptionEntry.h ../src/GLEXT/GLVertexArray.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/GLW/GLWPanel.h ../src/common/OptionsParam.h ../src/GLW/GLWVisibleWidget.h ../src/tankgraph/GLWTankViewer.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/dialogs/PlayerDialog.cpp ../src/client/ClientState.h ../src/GLW/GLWTextBox.h ../src/GLW/GLWWindow.h ../src/common/ARGParser.h ../src/GLW/GLWTextButton.h
../src/scorched/Release/srcscorchedMainDialogscorched.o: ../src/scorched/SingleDialog.h ../src/scorched/MainDialog.h ../src/scorched/NetDialog.h ../src/common/FileLines.h ../src/scorched/ServerSDialog.h ../src/common/Defines.h ../src/server/ServerMain.h ../src/common/OptionEntry.h ../src/common/OptionsParam.h ../src/scorched//MainDialog.cpp ../src/scorched/DisplayDialog.h ../src/coms/NetBuffer.h ../src/common/ARGParser.h
../src/scorched/Release/srcactionsShotProjectileExplosionscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/ode/contact.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/actions/ShotProjectileExplosion.h ../src/ode/misc.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/actions/ShotProjectileExplosion.cpp ../src/engine/MainLoopI.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/engine/GameState.h ../src/actions/Explosion.h ../src/coms/NetBuffer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/ode/config.h ../src/ode/odecpp.h ../src/engine/ActionController.h
../src/scorched/Release/srcclientClientMainscorched.o: ../src/common/Vector.h ../src/GLEXT/GLCamera.h ../src/common/FileLines.h ../src/common/Line.h ../src/GLEXT/GLMenuI.h ../src/common/Keyboard.h ../src/engine/GameStateI.h ../src/client/ClientMain.cpp ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/common/Gamma.h ../src/client/MainCamera.h ../src/common/Sound.h ../src/common/OptionEntry.h ../src/coms/NetClient.h ../src/GLEXT/GLState.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/common/OptionsParam.h ../src/common/Timer.h ../src/common/KeyboardHistory.h ../src/client/ClientDialog.h ../src/common/OptionsGame.h ../src/engine/GameState.h ../src/common/Mouse.h ../src/coms/NetBuffer.h ../src/coms/NetMessageHandler.h ../src/common/Display.h ../src/common/ARGParser.h ../src/engine/MainLoop.h
../src/scorched/Release/srcodemiscscorched.o: ../src/ode/misc.cpp ../src/ode/misc.h ../src/ode/matrix.h ../src/ode/common.h ../src/ode/error.h ../src/ode/config.h
../src/scorched/Release/srctankaiTankAIComputerPSharkscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/tankai/TankAIComputerChooser.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/tankai/TankAIComputerShooter.h ../src/tankai/TankAIComputerPShark.cpp ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/tankai/TankAIComputer.h ../src/tankai/TankAIComputerTosser.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tankai/TankAIComputerBuyer.h ../src/tank/TankFuel.h ../src/tankai/TankAIComputerPShark.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcserverServerBuyAccessoryHandlerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/server/ServerShotHolder.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/server/ServerState.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/server/ServerBuyAccessoryHandler.cpp ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/ode/matrix.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/coms/ComsPlayedMoveMessage.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/coms/ComsBuyAccessoryMessage.h ../src/tank/TankFuel.h ../src/server/ServerBuyAccessoryHandler.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srctankTankShieldsscorched.o: ../src/common/Vector.h ../src/tank/TankShields.h ../src/weapons/Accessory.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankShields.cpp
../src/scorched/Release/srczlibzutilscorched.o: ../src/zlib/zutil.c
../src/scorched/Release/srctankgraphGLWTankViewerscorched.o: ../src/GLEXT/GLImage.h ../src/tank/TankModelId.h ../src/GLW/GLWScrollW.h ../src/tankgraph/TankMesh.h ../src/common/Vector.h ../src/tankgraph/TankModel.h ../src/GLW/GLWScrollButton.h ../src/GLW/GLWidget.h ../src/GLEXT/GLFont2d.h ../src/GLW/GLWPanel.h ../src/tankgraph/GLWTankViewer.cpp ../src/GLW/GLWVisibleWidget.h ../src/common/Defines.h ../src/GLEXT/GLTexture.h ../src/tankgraph/GLWTankViewer.h ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/tankgraph/TankModelStore.h ../src/GLW/GLWButton.h ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLVertexArray.h ../src/GLEXT/GLState.h
../src/scorched/Release/srcodemassscorched.o: ../src/ode/mass.cpp ../src/ode/odemath.h ../src/ode/mass.h ../src/ode/matrix.h ../src/ode/common.h ../src/ode/error.h ../src/ode/config.h
../src/scorched/Release/srccommonSoundBufferscorched.o: ../src/common/Sound.h ../src/common/SoundBuffer.cpp ../src/common/SoundBuffer.h
../src/scorched/Release/srctankTankParachutesscorched.o: ../src/tank/TankParachutes.cpp ../src/coms/NetBuffer.h ../src/tank/TankParachutes.h
../src/scorched/Release/srcenginePhysicsParticlescorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/ode/contact.h ../src/ode/rotation.h ../src/engine/PhysicsParticle.h ../src/ode/memory.h ../src/engine/Action.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/engine/PhysicsParticle.cpp ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/ode/collision.h ../src/ode/space.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/coms/NetBuffer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/ode/config.h ../src/ode/odecpp.h
../src/scorched/Release/srclandscapeWaterMapSurroundscorched.o: ../src/common/Vector.h ../src/landscape/WaterMapSurround.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/landscape/WaterMapSurround.cpp
../src/scorched/Release/srczlibcrc32scorched.o: ../src/zlib/crc32.c
../src/scorched/Release/srcdialogsScoreDialogscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/GLW/GLWidget.h ../src/common/FileLines.h ../src/tank/TankSort.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/GLEXT/GLTexture.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/dialogs/ScoreDialog.cpp ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/GLW/GLWVisiblePanel.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/GLW/GLWPanel.h ../src/common/OptionsParam.h ../src/GLW/GLWVisibleWidget.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/GLEXT/GLImage.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/client/ClientState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/GLEXT/GLFont2d.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/GLW/GLWWindow.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/dialogs/ScoreDialog.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srctankTankAutoDefensescorched.o: ../src/tank/TankAutoDefense.h ../src/tank/TankAutoDefense.cpp ../src/coms/NetBuffer.h
../src/scorched/Release/srccomsNetServerscorched.o: ../src/common/Defines.h ../src/coms/NetServer.cpp ../src/coms/NetMessage.h ../src/coms/NetServer.h ../src/coms/NetBuffer.h ../src/coms/NetMessageHandler.h ../src/common/Logger.h ../src/coms/NetBufferUtil.h ../src/coms/NetMessagePool.h
../src/scorched/Release/srclandscapeMovementMapscorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/landscape/PatchGrid.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/GLEXT/GLTexture.h ../src/engine/ScorchedCollisionIds.h ../src/landscape/WaterMapSurround.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/landscape/Landscape.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/ode/collision_space.h ../src/landscape/SurroundDefs.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/landscape/Patch.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/landscape/TriNode.h ../src/ode/collision.h ../src/ode/space.h ../src/landscape/HeightMapSurround.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/landscape/WaterMapPoints.h ../src/coms/ComsMessage.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/landscape/ShadowMap.h ../src/landscape/MovementMap.cpp ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/GLEXT/GLImage.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/ode/geom.h ../src/landscape/PatchSide.h ../src/ode/common.h ../src/coms/ComsLevelMessage.h ../src/ode/objects.h ../src/ode/error.h ../src/GLEXT/GLTextureCubeMap.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/landscape/WaterMap.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/ode/odecpp.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h ../src/landscape/MovementMap.h ../src/tank/TankScore.h
../src/scorched/Release/srcclientClientGameStoppedHandlerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/coms/ComsGameStoppedMessage.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/client/ClientGameStoppedHandler.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/client/ClientState.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/client/ClientGameStoppedHandler.cpp ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/src3dsparseaseFile.lexscorched.o: ../src/common/Vector.h ../src/3dsparse/aseFile.lex.cpp ../src/3dsparse/aseFile.tab.cpp.h
../src/scorched/Release/srcGLEXTGLMenuIscorched.o: ../src/GLEXT/GLMenuI.h ../src/GLEXT/GLMenuI.cpp
../src/scorched/Release/srcodearrayscorched.o: ../src/ode/memory.h ../src/ode/error.h ../src/ode/config.h ../src/ode/array.cpp
../src/scorched/Release/srccomsComsPlayerStateMessagescorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/coms/ComsPlayerStateMessage.cpp ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/coms/ComsPlayerStateMessage.h ../src/tank/TankScore.h
../src/scorched/Release/srctankaiTankAIStringsscorched.o: ../src/common/FileLines.h ../src/tankai/TankAIStrings.h ../src/common/Defines.h ../src/tankai/TankAIStrings.cpp
../src/scorched/Release/srczlibinffastscorched.o: ../src/zlib/inffast.c
../src/scorched/Release/srctankTankStatescorched.o: ../src/coms/NetBuffer.h ../src/tank/TankState.h ../src/tank/TankState.cpp
../src/scorched/Release/srcclientShotTimerscorched.o: ../src/common/Vector.h ../src/client/ShotTimer.h ../src/engine/GameStateStimulusI.h ../src/common/FileLines.h ../src/GLEXT/GLTexture.h ../src/engine/GameStateI.h ../src/common/Defines.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/common/OptionsGame.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/client/ShotTimer.cpp ../src/GLEXT/GLImage.h ../src/GLEXT/GLFont2d.h ../src/common/ARGParser.h
../src/scorched/Release/srcGLEXTGLMenuEntryscorched.o: ../src/GLEXT/GLMenuEntry.cpp ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/GLEXT/GLTexture.h ../src/GLEXT/GLMenuI.h ../src/GLEXT/GLMenuEntry.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLFont2d.h
../src/scorched/Release/srcodeobstackscorched.o: ../src/ode/memory.h ../src/ode/obstack.cpp ../src/ode/common.h ../src/ode/error.h ../src/ode/config.h
../src/scorched/Release/srcactionsShotProjectileTracerscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/actions/ShotProjectileTracer.cpp ../src/sprites/TracerActionRenderer.h ../src/common/FileLines.h ../src/ode/contact.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/ode/memory.h ../src/engine/Action.h ../src/GLEXT/GLMenuI.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/common/Defines.h ../src/ode/mass.h ../src/tankgraph/TracerStore.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/engine/MainLoopI.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/tankgraph/TankRenderer.h ../src/common/OptionsParam.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/ode/geom.h ../src/ode/common.h ../src/actions/ShotProjectileTracer.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/tankgraph/TankMenus.h
../src/scorched/Release/srclandscapeHeightMapSurroundscorched.o: ../src/common/Vector.h ../src/landscape/HeightMapSurround.cpp ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/landscape/SurroundDefs.h ../src/GLEXT/GLState.h ../src/landscape/HeightMapSurround.h ../src/common/ProgressCounter.h
../src/scorched/Release/srccomsComsMessageHandlerscorched.o: ../src/coms/ComsMessageHandler.h ../src/coms/ComsMessageHandler.cpp ../src/common/Defines.h ../src/coms/NetMessage.h ../src/coms/NetBuffer.h ../src/coms/NetMessageHandler.h
../src/scorched/Release/srcdialogsProgressDialogscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/GLEXT/GLTexture.h ../src/GLW/GLWProgress.h ../src/common/Defines.h ../src/GLW/GLWVisiblePanel.h ../src/GLW/GLWLabel.h ../src/GLEXT/GLState.h ../src/dialogs/BackdropDialog.h ../src/engine/MainLoopI.h ../src/dialogs/ProgressDialog.cpp ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/common/Timer.h ../src/common/KeyboardHistory.h ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/dialogs/ProgressDialog.h ../src/GLW/GLWWindow.h ../src/engine/MainLoop.h
../src/scorched/Release/srcspritesFlareActionRendererscorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/landscape/PatchGrid.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/common/FileLines.h ../src/ode/contact.h ../src/sprites/FlareActionRenderer.cpp ../src/GLEXT/GLTexture.h ../src/ode/rotation.h ../src/engine/PhysicsParticle.h ../src/landscape/WaterMapSurround.h ../src/ode/memory.h ../src/engine/Action.h ../src/landscape/Landscape.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/ode/mass.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/landscape/SurroundDefs.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/landscape/Patch.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/landscape/TriNode.h ../src/landscape/HeightMapSurround.h ../src/ode/collision.h ../src/ode/space.h ../src/coms/ComsMessage.h ../src/landscape/WaterMapPoints.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/landscape/ShadowMap.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/landscape/PatchSide.h ../src/ode/geom.h ../src/coms/ComsLevelMessage.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/GLEXT/GLTextureCubeMap.h ../src/sprites/FlareActionRenderer.h ../src/landscape/WaterMap.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/landscape/MovementMap.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h ../src/GLEXT/GLLenseFlare.h
../src/scorched/Release/srctankTankStartscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/common/RandomGenerator.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/actions/ShowScore.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/tank/TankStart.cpp ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/actions/TankScored.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/coms/ComsLevelMessage.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankStart.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/landscape/MovementMap.h ../src/engine/ActionController.h ../src/tank/TankScore.h
../src/scorched/Release/srcspritesTalkRendererscorched.o: ../src/common/Vector.h ../src/GLEXT/GLTexture.h ../src/sprites/TalkRenderer.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/common/Defines.h ../src/sprites/TalkRenderer.cpp ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/GLEXT/GLCameraFrustum.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srctankgraphTracerStorescorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/tankgraph/TracerStore.cpp ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/tankgraph/TracerStore.h ../src/ode/collision_space.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcXMLxmltokscorched.o: ../src/XML/expat_config.h ../src/XML/xmltok.c
../src/scorched/Release/srcodememoryscorched.o: ../src/ode/memory.h ../src/ode/memory.cpp ../src/ode/error.h ../src/ode/config.h
../src/scorched/Release/srctankaiTankAIComputerChooserscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tankai/TankAIComputerChooser.cpp ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/tankai/TankAIComputerChooser.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/tankai/TankAIComputerShooter.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tankai/TankAIComputer.h ../src/tankai/TankAIComputerTosser.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tankai/TankAIComputerBuyer.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srccommonMousescorched.o: ../src/common/Vector.h ../src/common/Mouse.cpp ../src/engine/MainLoopI.h ../src/engine/GameState.h ../src/common/Mouse.h
../src/scorched/Release/srcserverServerMainscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/ode/contact.h ../src/common/FileLines.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/tank/TankShields.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/server/ServerPlayerReadyHandler.h ../src/landscape/HeightMapCollision.h ../src/server/ServerState.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/engine/ScorchedCollisionHandler.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/server/ServerDefenseHandler.h ../src/common/OptionsParam.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/common/Timer.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/server/ServerMain.cpp ../src/engine/PhysicsEngine.h ../src/common/OptionsGame.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/coms/NetServer.h ../src/server/ServerTextHandler.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/ode/geom.h ../src/coms/ComsLevelMessage.h ../src/ode/common.h ../src/server/ServerPlayedMoveHandler.h ../src/ode/objects.h ../src/ode/error.h ../src/server/ServerBuyAccessoryHandler.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/tankai/TankAIAdder.h ../src/common/ARGParser.h ../src/server/ServerMessageHandler.h ../src/scorched/ServerDialog.h ../src/ode/odecpp.h ../src/landscape/MovementMap.h ../src/server/ServerConnectHandler.h ../src/tank/TankScore.h ../src/engine/ActionController.h
../src/scorched/Release/srclandscapeDeformLandscapescorched.o: ../src/common/Vector.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/landscape/DeformLandscape.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/landscape/DeformLandscape.cpp ../src/coms/NetMessage.h ../src/coms/ComsMessage.h ../src/coms/NetBuffer.h ../src/common/ProgressCounter.h ../src/coms/ComsLevelMessage.h ../src/landscape/MovementMap.h
../src/scorched/Release/srctankgraphTankMenusscorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/landscape/PatchGrid.h ../src/GLW/GLWidget.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/GLEXT/GLTexture.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/dialogs/MainMenuDialog.h ../src/engine/ScorchedCollisionIds.h ../src/landscape/WaterMapSurround.h ../src/tank/TankAutoDefense.h ../src/GLW/GLWButton.h ../src/ode/memory.h ../src/GLEXT/GLMenuI.h ../src/landscape/Landscape.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/GLEXT/GLMenuEntry.h ../src/ode/odecpp_collision.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/landscape/SurroundDefs.h ../src/ode/collision_space.h ../src/GLW/GLWVisiblePanel.h ../src/dialogs/QuitDialog.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/landscape/Patch.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/landscape/TriNode.h ../src/landscape/HeightMapSurround.h ../src/ode/space.h ../src/ode/collision.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/landscape/WaterMapPoints.h ../src/weapons/Accessory.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/landscape/ShadowMap.h ../src/engine/PhysicsEngine.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/tankgraph/TankMenus.cpp ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/GLEXT/GLImage.h ../src/common/WindowManager.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/tank/TankContainer.h ../src/client/ClientState.h ../src/landscape/PatchSide.h ../src/ode/geom.h ../src/coms/ComsLevelMessage.h ../src/ode/common.h ../src/ode/error.h ../src/ode/objects.h ../src/GLEXT/GLFont2d.h ../src/GLEXT/GLTextureCubeMap.h ../src/tank/TankFuel.h ../src/GLEXT/GLMenu.h ../src/tank/TankParachutes.h ../src/GLW/GLWWindow.h ../src/landscape/WaterMap.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/ode/odecpp.h ../src/landscape/MovementMap.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h ../src/tankgraph/TankMenus.h ../src/tank/TankScore.h
../src/scorched/Release/srclandscapeHeightMapRendererscorched.o: ../src/common/Vector.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/landscape/HeightMapRenderer.h ../src/common/ProgressCounter.h ../src/landscape/HeightMapRenderer.cpp
../src/scorched/Release/srcweaponsParachutescorched.o: ../src/weapons/Parachute.h ../src/common/Defines.h ../src/weapons/Accessory.h ../src/weapons/Parachute.cpp
../src/scorched/Release/srcactionsShotProjectileRiotscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/ode/contact.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/ode/memory.h ../src/engine/Action.h ../src/actions/ShotProjectileRiot.cpp ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/engine/MainLoopI.h ../src/actions/ShotProjectileRiot.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/engine/GameState.h ../src/actions/Explosion.h ../src/coms/NetBuffer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/ode/config.h ../src/ode/odecpp.h ../src/engine/ActionController.h
../src/scorched/Release/srcclientClientNewGameHandlerscorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/client/ClientNewGameHandler.h ../src/landscape/PatchGrid.h ../src/GLW/GLWidget.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/ode/contact.h ../src/common/FileLines.h ../src/GLEXT/GLFontBanner.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/GLEXT/GLTexture.h ../src/GLW/GLWProgress.h ../src/engine/ScorchedCollisionIds.h ../src/landscape/WaterMapSurround.h ../src/tank/TankAutoDefense.h ../src/GLW/GLWButton.h ../src/common/RandomGenerator.h ../src/ode/memory.h ../src/GLEXT/GLMenuI.h ../src/landscape/Landscape.h ../src/ode/odemath.h ../src/GLW/GLWScrollW.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/dialogs/ConnectDialog.h ../src/ode/odecpp_collision.h ../src/GLW/GLWScrollButton.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/tankgraph/TracerStore.h ../src/ode/collision_space.h ../src/landscape/SurroundDefs.h ../src/GLW/GLWVisiblePanel.h ../src/GLW/GLWLabel.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/landscape/Patch.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/landscape/TriNode.h ../src/ode/collision.h ../src/ode/space.h ../src/landscape/HeightMapSurround.h ../src/GLW/GLWPanel.h ../src/tankgraph/TankRenderer.h ../src/GLW/GLWVisibleWidget.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/landscape/WaterMapPoints.h ../src/landscape/Smoke.h ../src/common/Timer.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/sprites/ExplosionTextures.h ../src/engine/PhysicsEngine.h ../src/landscape/ShadowMap.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/client/SpeedChange.h ../src/tank/TankModelId.h ../src/GLEXT/GLImage.h ../src/client/MainBanner.h ../src/client/ClientNewGameHandler.cpp ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/tank/TankContainer.h ../src/client/ClientState.h ../src/ode/geom.h ../src/landscape/PatchSide.h ../src/ode/common.h ../src/coms/ComsLevelMessage.h ../src/ode/objects.h ../src/ode/error.h ../src/GLW/GLWTextBox.h ../src/GLEXT/GLTextureCubeMap.h ../src/tank/TankFuel.h ../src/dialogs/ProgressDialog.h ../src/common/Logger.h ../src/tank/TankStart.h ../src/GLW/GLWListView.h ../src/tank/TankParachutes.h ../src/landscape/WaterMap.h ../src/GLW/GLWWindow.h ../src/coms/ComsNewGameMessage.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/ode/odecpp.h ../src/GLEXT/GLTextureSet.h ../src/landscape/MovementMap.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h ../src/GLW/GLWTextButton.h ../src/engine/MainLoop.h ../src/tankgraph/TankMenus.h ../src/GLEXT/GLLenseFlare.h ../src/tank/TankScore.h
../src/scorched/Release/srcactionsShotProjectilescorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/tank/TankShields.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/ode/odemath.h ../src/actions/ShotProjectile.cpp ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/common/OptionsParam.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/tank/TankFuel.h ../src/sprites/FlareActionRenderer.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srclandscapeGlobalHMapscorched.o: ../src/common/Vector.h ../src/zlib/zlib.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/common/FileLines.h ../src/common/RandomGenerator.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/common/OptionEntry.h ../src/coms/NetMessage.h ../src/coms/ComsMessage.h ../src/common/OptionsGame.h ../src/landscape/HeightMapModifier.h ../src/coms/NetBuffer.h ../src/landscape/GlobalHMap.cpp ../src/common/ProgressCounter.h ../src/coms/ComsLevelMessage.h ../src/common/ARGParser.h ../src/landscape/MovementMap.h
../src/scorched/Release/srcGLWGLWTextBoxscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/GLEXT/GLTexture.h ../src/common/Keyboard.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWTextBox.cpp ../src/GLW/GLWVisibleWidget.h ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/GLEXT/GLImage.h ../src/GLW/GLWTextBox.h ../src/GLEXT/GLFont2d.h
../src/scorched/Release/srcactionsShotProjectileClodscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/ode/contact.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/actions/ShotProjectileClod.cpp ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/engine/MainLoopI.h ../src/actions/ShotProjectile.h ../src/actions/ShotProjectileClod.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/engine/GameState.h ../src/actions/Explosion.h ../src/coms/NetBuffer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/ode/config.h ../src/ode/odecpp.h ../src/engine/ActionController.h
../src/scorched/Release/srclandscapePatchVarscorched.o: ../src/landscape/PatchVar.cpp ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLEXT/GLVar.h ../src/landscape/PatchVar.h
../src/scorched/Release/srccommonOptionsParamscorched.o: ../src/common/FileLines.h ../src/common/OptionsParam.cpp ../src/common/Defines.h ../src/common/OptionEntry.h ../src/common/OptionsParam.h ../src/coms/NetBuffer.h ../src/common/ARGParser.h
../src/scorched/Release/srctankTankAIscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/tank/TankAI.cpp ../src/coms/ComsDefenseMessage.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/server/ServerShotHolder.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/common/OptionsParam.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/client/ClientState.h ../src/coms/ComsPlayedMoveMessage.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/tank/TankAILogic.h ../src/tank/TankScore.h
../src/scorched/Release/srcclientWindowSetupscorched.o: ../src/common/Vector.h ../src/dialogs/PlayerDialog.h ../src/GLW/GLWidget.h ../src/common/FileLines.h ../src/dialogs/WindDialog.h ../src/dialogs/AutoDefenseDialog.h ../src/GLEXT/GLTexture.h ../src/GLW/GLWTab.h ../src/dialogs/MainMenuDialog.h ../src/GLW/GLWFlag.h ../src/GLW/GLWButton.h ../src/dialogs/PlayersDialog.h ../src/GLEXT/GLMenuI.h ../src/GLW/GLWScrollW.h ../src/common/Keyboard.h ../src/engine/GameStateI.h ../src/dialogs/HelpDialog.h ../src/dialogs/ConnectDialog.h ../src/GLEXT/GLMenuEntry.h ../src/dialogs/TalkDialog.h ../src/GLW/GLWScrollButton.h ../src/GLW/GLWDropDown.h ../src/common/Defines.h ../src/GLW/GLWVisiblePanel.h ../src/GLW/GLWLabel.h ../src/GLEXT/GLVertexArray.h ../src/dialogs/QuitDialog.h ../src/GLEXT/GLState.h ../src/dialogs/BackdropDialog.h ../src/dialogs/PlanViewDialog.h ../src/dialogs/KibitzingDialog.h ../src/engine/MainLoopI.h ../src/dialogs/KillDialog.h ../src/client/WindowSetup.cpp ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/tankgraph/GLWTankViewer.h ../src/common/KeyboardHistory.h ../src/GLW/GLWPushButton.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLImage.h ../src/common/WindowManager.h ../src/client/ClientState.h ../src/dialogs/TankDialog.h ../src/GLW/GLWTextBox.h ../src/GLEXT/GLFont2d.h ../src/client/WindowSetup.h ../src/common/Logger.h ../src/GLW/GLWListView.h ../src/GLEXT/GLMenu.h ../src/GLW/GLWWindow.h ../src/dialogs/ScoreDialog.h ../src/dialogs/BuyAccessoryDialog.h ../src/GLW/GLWScrollPanel.h ../src/GLW/GLWTextButton.h
../src/scorched/Release/srcGLEXTGLTexturescorched.o: ../src/GLEXT/GLTexture.h ../src/common/Defines.h ../src/GLEXT/GLStateExtension.h ../src/GLEXT/GLState.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLTexture.cpp
../src/scorched/Release/srcweaponsWeaponClodscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/PhysicsParticle.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/actions/ShotProjectile.h ../src/actions/ShotProjectileClod.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/weapons/WeaponClod.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/weapons/WeaponClod.cpp ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srctankaiTankAIComputerDHTestscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/tankai/TankAIComputerDHTest.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/tankai/TankAIComputerChooser.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/tankai/TankAIComputerShooter.h ../src/ode/collision.h ../src/ode/space.h ../src/tankai/TankAIComputerDHTest.cpp ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tankai/TankAIComputer.h ../src/tankai/TankAIComputerTosser.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tankai/TankAIComputerBuyer.h ../src/tank/TankFuel.h ../src/tankai/TankAIComputerPShark.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srclandscapeDeformTexturesscorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/common/Vector.h ../src/landscape/PatchGrid.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/GLEXT/GLTexture.h ../src/landscape/DeformTextures.h ../src/landscape/WaterMapSurround.h ../src/landscape/DeformLandscape.h ../src/landscape/Landscape.h ../src/engine/GameStateI.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/GLEXT/GLBitmapModifier.h ../src/common/Defines.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/landscape/SurroundDefs.h ../src/GLEXT/GLState.h ../src/landscape/Patch.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/landscape/TriNode.h ../src/landscape/HeightMapSurround.h ../src/coms/ComsMessage.h ../src/landscape/WaterMapPoints.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/landscape/ShadowMap.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/landscape/PatchSide.h ../src/coms/ComsLevelMessage.h ../src/GLEXT/GLTextureCubeMap.h ../src/landscape/DeformTextures.cpp ../src/landscape/WaterMap.h ../src/common/OptionsTransient.h ../src/landscape/MovementMap.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcserverServerPlayerReadyHandlerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/server/ServerPlayerReadyHandler.h ../src/tank/TankAutoDefense.h ../src/server/ServerState.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/coms/ComsPlayerReadyMessage.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/server/ServerPlayerReadyHandler.cpp ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcGLWGLWFlagscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/GLW/GLWFlag.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h ../src/GLW/GLWFlag.cpp
../src/scorched/Release/srcweaponsAutoDefensescorched.o: ../src/common/Vector.h ../src/weapons/Accessory.h ../src/weapons/AutoDefense.cpp ../src/coms/NetBuffer.h ../src/common/OptionsTransient.h ../src/weapons/AutoDefense.h
../src/scorched/Release/srccommonARGParserscorched.o: ../src/common/Defines.h ../src/common/ARGParser.cpp ../src/common/ARGParser.h
../src/scorched/Release/srccomsComsStartGameMessagescorched.o: ../src/common/Vector.h ../src/coms/ComsStartGameMessage.cpp ../src/coms/NetMessage.h ../src/coms/ComsMessage.h ../src/coms/NetBuffer.h ../src/common/OptionsTransient.h ../src/coms/ComsStartGameMessage.h ../src/coms/ComsPlayerStateMessage.h
../src/scorched/Release/srcGLWGLWScrollPanelscorched.o: ../src/GLW/GLWidget.h ../src/GLW/GLWScrollPanel.cpp ../src/GLW/GLWButton.h ../src/GLW/GLWScrollW.h ../src/GLW/GLWScrollButton.h ../src/common/Defines.h ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLState.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h ../src/GLW/GLWScrollPanel.h
../src/scorched/Release/srccomsComsConnectMessagescorched.o: ../src/coms/ComsConnectMessage.h ../src/coms/NetMessage.h ../src/coms/ComsMessage.h ../src/coms/ComsConnectMessage.cpp ../src/coms/NetBuffer.h
../src/scorched/Release/srcGLEXTGLTextureSetscorched.o: ../src/GLEXT/GLTexture.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLEXT/GLTextureSet.cpp ../src/GLEXT/GLImage.h ../src/GLEXT/GLTextureSet.h
../src/scorched/Release/srcclientClientDefenseHandlerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/coms/ComsDefenseMessage.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/client/ClientDefenseHandler.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/coms/ComsMessage.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/coms/ComsPlayedMoveMessage.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/client/ClientDefenseHandler.cpp ../src/ode/odecpp.h ../src/tank/TankAILogic.h ../src/tank/TankScore.h
../src/scorched/Release/srcweaponsShieldscorched.o: ../src/common/Vector.h ../src/common/Defines.h ../src/weapons/Shield.cpp ../src/weapons/Accessory.h ../src/common/VectorLib.h ../src/weapons/Shield.h
../src/scorched/Release/srcactionsTankMovescorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/actions/TankMove.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/actions/TankMove.cpp ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcweaponsWeaponProjectilescorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/tank/TankShields.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/weapons/WeaponProjectile.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/actions/ShotProjectileExplosion.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/weapons/WeaponProjectile.cpp ../src/tank/TankScore.h
../src/scorched/Release/srcGLEXTGLLenseFlarescorched.o: ../src/common/Vector.h ../src/GLEXT/GLCamera.h ../src/GLEXT/GLTexture.h ../src/common/Line.h ../src/GLEXT/GLMenuI.h ../src/engine/GameStateI.h ../src/common/Defines.h ../src/client/MainCamera.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/GLEXT/GLLuminance.h ../src/GLEXT/GLLenseFlare.cpp ../src/GLEXT/GLCameraFrustum.h ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/GLEXT/GLLenseFlare.h
../src/scorched/Release/srcGLEXTGLImagescorched.o: ../src/common/Defines.h ../src/GLEXT/GLImage.cpp ../src/GLEXT/GLState.h ../src/GLEXT/GLImage.h
../src/scorched/Release/srctankTankModelIdscorched.o: ../src/tank/TankModelId.cpp ../src/tank/TankModelId.h
../src/scorched/Release/srcserverScorchedASEscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/common/FileLines.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/server/ServerState.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/server/ScorchedASE.cpp ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/common/OptionsGame.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetServer.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcactionsTankFallingscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/actions/TankFalling.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/tank/TankShields.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/actions/TankMove.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/tank/TankController.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/actions/TankFalling.cpp ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/engine/ActionController.h ../src/tank/TankScore.h
../src/scorched/Release/srcdialogsKibitzingDialogscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/GLW/GLWButton.h ../src/GLEXT/GLMenuI.h ../src/dialogs/KibitzingDialog.cpp ../src/engine/GameStateI.h ../src/common/Defines.h ../src/GLW/GLWVisiblePanel.h ../src/GLW/GLWLabel.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/dialogs/KibitzingDialog.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/common/WindowManager.h ../src/GLW/GLWWindow.h ../src/GLW/GLWTextButton.h
../src/scorched/Release/srcGLWGLWScrollButtonscorched.o: ../src/GLW/GLWidget.h ../src/GLW/GLWButton.h ../src/GLW/GLWScrollButton.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWScrollButton.cpp ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h
../src/scorched/Release/srccommonTrianglescorched.o: ../src/common/Triangle.cpp ../src/common/Vector.h ../src/common/Line.h ../src/common/Defines.h ../src/common/Triangle.h
../src/scorched/Release/srclandscapeNapalmMapscorched.o: ../src/landscape/NapalmMap.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/landscape/NapalmMap.cpp
../src/scorched/Release/srcscorchedCreateDialogsscorched.o: ../src/scorched/MainDialog.h ../src/common/FileLines.h ../src/common/Defines.h ../src/scorched//CreateDialogs.cpp ../src/server/ServerMain.h ../src/common/OptionEntry.h ../src/coms/NetMessage.h ../src/common/OptionsParam.h ../src/common/OptionsGame.h ../src/coms/NetBuffer.h ../src/common/ARGParser.h ../src/scorched/ServerDialog.h
../src/scorched/Release/srctankTankColorGeneratorscorched.o: ../src/common/Vector.h ../src/tank/TankColorGenerator.cpp ../src/tank/TankColorGenerator.h
../src/scorched/Release/srcXMLXMLFilescorched.o: ../src/XML/XMLFile.h ../src/common/FileLines.h ../src/XML/XMLParser.h ../src/XML/XMLFile.cpp ../src/XML/expat.h
../src/scorched/Release/srccomsComsGameStoppedMessagescorched.o: ../src/coms/ComsGameStoppedMessage.cpp ../src/coms/NetMessage.h ../src/coms/ComsGameStoppedMessage.h ../src/coms/ComsMessage.h ../src/coms/NetBuffer.h
../src/scorched/Release/srcGLWGLWPanelscorched.o: ../src/GLW/GLWidget.h ../src/GLW/GLWPanel.cpp ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWPanel.h ../src/common/KeyboardHistory.h
../src/scorched/Release/srccommonFileLinesscorched.o: ../src/common/FileLines.h ../src/common/FileLines.cpp
../src/scorched/Release/srcweaponsWeaponRiotBombscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/weapons/WeaponRiotBomb.cpp ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/PhysicsParticle.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/actions/ShotProjectileRiot.h ../src/tank/TankBatteries.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/weapons/WeaponRiotBomb.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcactionsNapalmscorched.o: ../src/landscape/TriNodePool.h ../src/engine/ActionMeta.h ../src/landscape/Wall.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/landscape/PatchGrid.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/GLEXT/GLTexture.h ../src/engine/ScorchedCollisionIds.h ../src/landscape/WaterMapSurround.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/landscape/Landscape.h ../src/ode/odemath.h ../src/engine/GameStateI.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/tank/TankController.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/ode/collision_space.h ../src/landscape/SurroundDefs.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/landscape/Patch.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/landscape/TriNode.h ../src/ode/collision.h ../src/ode/space.h ../src/landscape/HeightMapSurround.h ../src/actions/Napalm.cpp ../src/common/OptionsParam.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/landscape/WaterMapPoints.h ../src/coms/ComsMessage.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/landscape/ShadowMap.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/GLEXT/GLImage.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/landscape/PatchSide.h ../src/ode/common.h ../src/coms/ComsLevelMessage.h ../src/ode/objects.h ../src/ode/error.h ../src/GLEXT/GLTextureCubeMap.h ../src/tank/TankFuel.h ../src/actions/Napalm.h ../src/tank/TankParachutes.h ../src/landscape/WaterMap.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h ../src/landscape/MovementMap.h ../src/tank/TankScore.h
../src/scorched/Release/srccomsComsMessageSenderscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/coms/ComsMessageSender.cpp ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/coms/NetClient.h ../src/ode/timer.h ../src/tank/Tank.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetServer.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srccomsComsPlayerReadyMessagescorched.o: ../src/coms/NetMessage.h ../src/coms/ComsMessage.h ../src/coms/ComsPlayerReadyMessage.h ../src/coms/NetBuffer.h ../src/coms/ComsPlayerReadyMessage.cpp
../src/scorched/Release/srcGLWGLWWindowscorched.o: ../src/GLW/GLWidget.h ../src/GLEXT/GLTexture.h ../src/common/Defines.h ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLState.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/GLW/GLWWindow.cpp ../src/common/KeyboardHistory.h ../src/GLEXT/GLImage.h ../src/GLW/GLWWindow.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcactionsTankDeadscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/tank/TankShields.h ../src/ode/rotation.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/sprites/TankDeadRenderer.h ../src/ode/odecpp_collision.h ../src/actions/ShotProjectileExplosion.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/actions/TankDead.h ../src/ode/collision_space.h ../src/common/Sound.h ../src/common/OptionEntry.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/common/OptionsParam.h ../src/weapons/Weapon.h ../src/actions/TankDead.cpp ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/engine/GameState.h ../src/actions/Explosion.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/common/SoundStore.h ../src/tank/TankContainer.h ../src/actions/TankScored.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/common/SoundBuffer.h ../src/tank/TankParachutes.h ../src/actions/ShotProjectileFunky.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/engine/ActionController.h ../src/tank/TankScore.h
../src/scorched/Release/srcspritesWallActionRendererscorched.o: ../src/landscape/TriNodePool.h ../src/landscape/Wall.h ../src/common/Vector.h ../src/landscape/PatchGrid.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/GLEXT/GLTexture.h ../src/landscape/WaterMapSurround.h ../src/sprites/WallActionRenderer.h ../src/engine/Action.h ../src/landscape/Landscape.h ../src/engine/GameStateI.h ../src/landscape/Sun.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/sprites/WallActionRenderer.cpp ../src/landscape/SmokeChain.h ../src/landscape/Surround.h ../src/landscape/SurroundDefs.h ../src/GLEXT/GLState.h ../src/landscape/Patch.h ../src/coms/NetMessage.h ../src/engine/MainLoopI.h ../src/landscape/TriNode.h ../src/landscape/HeightMapSurround.h ../src/coms/ComsMessage.h ../src/landscape/WaterMapPoints.h ../src/landscape/Smoke.h ../src/common/KeyboardHistory.h ../src/landscape/ShadowMap.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/GLEXT/GLImage.h ../src/common/ProgressCounter.h ../src/landscape/PatchSide.h ../src/coms/ComsLevelMessage.h ../src/GLEXT/GLTextureCubeMap.h ../src/landscape/WaterMap.h ../src/common/OptionsTransient.h ../src/landscape/MovementMap.h ../src/landscape/Variance.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcGLEXTGLVertexArrayscorched.o: ../src/GLEXT/GLVertexArray.cpp ../src/common/Defines.h ../src/GLEXT/GLVertexArray.h ../src/GLEXT/GLStateExtension.h ../src/GLEXT/GLState.h
../src/scorched/Release/srctankTankAILogicscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/landscape/NapalmMap.h ../src/landscape/GlobalHMap.h ../src/coms/ComsDefenseMessage.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/weapons/Parachute.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/landscape/HeightMap.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/actions/TankMovement.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/Sound.h ../src/common/OptionEntry.h ../src/weapons/Battery.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/common/OptionsParam.h ../src/actions/TankResign.h ../src/weapons/Weapon.h ../src/actions/TankFired.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/common/OptionsGame.h ../src/engine/PhysicsEngine.h ../src/tank/TankAILogic.cpp ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/common/SoundStore.h ../src/coms/ComsPlayedMoveMessage.h ../src/ode/geom.h ../src/coms/ComsLevelMessage.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/SoundBuffer.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/landscape/MovementMap.h ../src/tank/TankAILogic.h ../src/engine/ActionController.h ../src/tank/TankScore.h
../src/scorched/Release/srczlibinflatescorched.o: ../src/zlib/inflate.c
../src/scorched/Release/srcGLWGLWCheckBoxscorched.o: ../src/GLW/GLWidget.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWCheckBox.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h ../src/GLW/GLWCheckBox.cpp
../src/scorched/Release/srctankaiTankAIComputerTosserscorched.o: ../src/tankai/TankAIComputerTosser.cpp ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/tank/TankLib.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/tankai/TankAIComputerShooter.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/weapons/AccessoryStore.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tankai/TankAIComputer.h ../src/tankai/TankAIComputerTosser.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tankai/TankAIComputerBuyer.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/common/OptionsTransient.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankScore.h
../src/scorched/Release/srcodespacescorched.o: ../src/ode/contact.h ../src/ode/memory.h ../src/ode/space.cpp ../src/ode/space.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/error.h ../src/ode/config.h
../src/scorched/Release/srcGLEXTGLConsoleLinesscorched.o: ../src/common/Vector.h ../src/GLEXT/GLConsoleLines.cpp ../src/GLEXT/GLTexture.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLFont2d.h ../src/GLEXT/GLConsoleLines.h
../src/scorched/Release/srcGLEXTGLVertexTexArrayscorched.o: ../src/GLEXT/GLVertexTexArray.h ../src/common/Defines.h ../src/GLEXT/GLVertexArray.h ../src/GLEXT/GLStateExtension.h ../src/GLEXT/GLState.h ../src/GLEXT/GLVertexTexArray.cpp
../src/scorched/Release/srcserverServerDefenseHandlerscorched.o: ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/coms/ComsDefenseMessage.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/coms/ComsMessageHandler.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/server/ServerShotHolder.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/server/ServerState.h ../src/ode/memory.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/server/ServerDefenseHandler.cpp ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/coms/ComsMessageSender.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/coms/NetMessage.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/server/ServerDefenseHandler.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/coms/ComsMessage.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/AccessoryStore.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/coms/NetMessageHandler.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/coms/ComsPlayedMoveMessage.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/ode/odecpp.h ../src/tank/TankAILogic.h ../src/tank/TankScore.h
../src/scorched/Release/srclandscapeWaterMapPointsscorched.o: ../src/common/Vector.h ../src/GLEXT/GLTexture.h ../src/landscape/WaterMapSurround.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/landscape/WaterMapPoints.h ../src/GLEXT/GLImage.h ../src/landscape/WaterMapPoints.cpp ../src/GLEXT/GLTextureCubeMap.h ../src/landscape/WaterMap.h ../src/GLEXT/GLBitmap.h
../src/scorched/Release/srcGLEXTGLConsolescorched.o: ../src/common/Vector.h ../src/GLEXT/GLTexture.h ../src/GLEXT/GLConsoleRuleFn.h ../src/engine/GameStateI.h ../src/common/Keyboard.h ../src/GLEXT/GLConsole.cpp ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/GLEXT/GLConsoleRuleMethod.h ../src/GLEXT/GLConsole.h ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLConsoleMethods.h ../src/GLEXT/GLConsoleRule.h ../src/GLEXT/GLFont2d.h ../src/GLEXT/GLConsoleLines.h ../src/GLEXT/GLConsoleRules.h
../src/scorched/Release/srclandscapeCloudSimscorched.o: ../src/landscape/CloudSim.h ../src/landscape/CloudSim.cpp ../src/common/Defines.h ../src/GLEXT/GLState.h
../src/scorched/Release/srcGLEXTGLMenuscorched.o: ../src/common/Vector.h ../src/GLW/GLWidget.h ../src/GLEXT/GLTexture.h ../src/GLEXT/GLMenuI.h ../src/GLEXT/GLMenuEntry.h ../src/common/Defines.h ../src/GLW/GLWVisiblePanel.h ../src/GLEXT/GLState.h ../src/GLW/GLWPanel.h ../src/GLW/GLWVisibleWidget.h ../src/GLEXT/GLMenu.cpp ../src/GLW/GLWFont.h ../src/common/KeyboardHistory.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLFont2d.h ../src/GLW/GLWWindow.h ../src/GLEXT/GLMenu.h
../src/scorched/Release/srcactionsTankSayscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/actions/TankSay.cpp ../src/GLEXT/GLTexture.h ../src/ode/rotation.h ../src/tank/TankShields.h ../src/sprites/TalkRenderer.h ../src/engine/ScorchedCollisionIds.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/engine/GameStateI.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/ode/mass.h ../src/actions/TankSay.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/engine/MainLoopI.h ../src/tank/TankBatteries.h ../src/ode/collision.h ../src/ode/space.h ../src/common/OptionsParam.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/common/KeyboardHistory.h ../src/ode/matrix.h ../src/engine/ActionBuffer.h ../src/engine/PhysicsEngine.h ../src/ode/ode.h ../src/tank/TankAccessories.h ../src/engine/GameState.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/GLEXT/GLImage.h ../src/tank/TankModelId.h ../src/tank/TankState.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/ode/objects.h ../src/ode/error.h ../src/tank/TankFuel.h ../src/common/Logger.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/engine/ActionController.h ../src/tank/TankScore.h
../src/scorched/Release/srcodefastdotscorched.o: ../src/ode/fastdot.c
../src/scorched/Release/srcGLEXTGLConsoleRuleMethodIAdapterscorched.o: ../src/common/Vector.h ../src/GLEXT/GLTexture.h ../src/GLEXT/GLConsoleRuleMethodIAdapter.h ../src/GLEXT/GLConsoleRuleFn.h ../src/engine/GameStateI.h ../src/common/Defines.h ../src/GLEXT/GLConsoleRuleMethodIAdapter.cpp ../src/GLEXT/GLState.h ../src/engine/MainLoopI.h ../src/GLEXT/GLConsoleRuleMethod.h ../src/GLEXT/GLConsole.h ../src/common/KeyboardHistory.h ../src/engine/GameState.h ../src/GLEXT/GLImage.h ../src/GLEXT/GLConsoleMethods.h ../src/GLEXT/GLConsoleRule.h ../src/GLEXT/GLFont2d.h ../src/GLEXT/GLConsoleLines.h ../src/GLEXT/GLConsoleRules.h
../src/scorched/Release/srcGLWGLWButtonscorched.o: ../src/GLW/GLWidget.h ../src/GLW/GLWButton.h ../src/common/Keyboard.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWVisibleWidget.h ../src/common/KeyboardHistory.h ../src/GLW/GLWButton.cpp
../src/scorched/Release/srcscorchedmainscorched.o: ../src/common/Vector.h ../src/client/ClientMain.h ../src/common/FileLines.h ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/common/OptionEntry.h ../src/common/OptionsParam.h ../src/common/OptionsGame.h ../src/coms/NetBuffer.h ../src/scorched//main.cpp ../src/common/Registry.h ../src/common/Resources.h ../src/common/OptionsTransient.h ../src/common/ARGParser.h ../src/common/ResourceFile.h
../src/scorched/Release/src3dsparseMeshLODscorched.o: ../src/common/Vector.h ../src/3dsparse/MeshLOD.cpp ../src/common/Face.h ../src/3dsparse/MeshLODTri.h ../src/3dsparse/MeshLODVector.h ../src/3dsparse/MeshLOD.h
../src/scorched/Release/srccommonSoundscorched.o: ../src/common/Sound.cpp ../src/common/Defines.h ../src/common/Sound.h ../src/common/SoundBuffer.h
../src/scorched/Release/srccomsNetBufferscorched.o: ../src/coms/NetBuffer.h ../src/coms/NetBuffer.cpp
../src/scorched/Release/srcweaponsShieldReflectiveMagscorched.o: ../src/common/Vector.h ../src/weapons/ShieldReflective.h ../src/weapons/ShieldReflectiveMag.cpp ../src/weapons/ShieldReflectiveMag.h ../src/weapons/Accessory.h ../src/weapons/Shield.h
../src/scorched/Release/srcGLEXTGLConsoleRuleFnscorched.o: ../src/GLEXT/GLConsoleRuleFn.h ../src/common/Defines.h ../src/GLEXT/GLConsoleRule.h ../src/GLEXT/GLConsoleRuleFn.cpp
../src/scorched/Release/srcweaponsBatteryscorched.o: ../src/weapons/Battery.cpp ../src/common/Defines.h ../src/weapons/Battery.h ../src/weapons/Accessory.h
../src/scorched/Release/srccommonFacescorched.o: ../src/common/Vector.h ../src/common/Face.h ../src/common/Face.cpp
../src/scorched/Release/srccommonKeyboardscorched.o: ../src/common/Keyboard.h ../src/common/KeyboardHistory.h ../src/common/Keyboard.cpp
../src/scorched/Release/srcodefastlsolvescorched.o: ../src/ode/fastlsolve.c
../src/scorched/Release/srcengineActionMetascorched.o: ../src/engine/ActionMeta.h ../src/engine/Action.h ../src/engine/ActionMeta.cpp ../src/common/Defines.h ../src/coms/NetBuffer.h
../src/scorched/Release/srclandscapePatchscorched.o: ../src/landscape/TriNodePool.h ../src/common/Vector.h ../src/landscape/HeightMap.h ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/landscape/Patch.h ../src/landscape/TriNode.h ../src/landscape/Patch.cpp ../src/common/ProgressCounter.h ../src/landscape/PatchSide.h ../src/landscape/Variance.h
../src/scorched/Release/srczlibinfcodesscorched.o: ../src/zlib/infcodes.c
../src/scorched/Release/srctankgraphTankModelscorched.o: ../src/tankgraph/TankModel.cpp ../src/common/Vector.h ../src/GLEXT/GLTexture.h ../src/common/Defines.h ../src/tankgraph/TankModelStore.h ../src/GLEXT/GLVertexArray.h ../src/GLEXT/GLState.h ../src/GLEXT/GLImage.h ../src/tank/TankModelId.h ../src/tankgraph/TankMesh.h ../src/tankgraph/TankModel.h
../src/scorched/Release/srccomsComsPlayedMoveMessagescorched.o: ../src/coms/ComsPlayedMoveMessage.cpp ../src/coms/NetMessage.h ../src/coms/ComsMessage.h ../src/coms/NetBuffer.h ../src/coms/ComsPlayedMoveMessage.h
../src/scorched/Release/srcspritesTracerActionRendererscorched.o: ../src/engine/ActionMeta.h ../src/common/Vector.h ../src/tank/TankPhysics.h ../src/sprites/TracerActionRenderer.h ../src/common/FileLines.h ../src/ode/contact.h ../src/tank/TankWeapon.h ../src/tank/TankShields.h ../src/ode/rotation.h ../src/GLEXT/GLTexture.h ../src/engine/ScorchedCollisionIds.h ../src/engine/PhysicsParticle.h ../src/tank/TankAutoDefense.h ../src/ode/memory.h ../src/engine/Action.h ../src/ode/odemath.h ../src/ode/odecpp_old_collision.h ../src/ode/odecpp_collision.h ../src/ode/misc.h ../src/tank/TankAI.h ../src/common/Defines.h ../src/common/OptionsDisplay.h ../src/ode/mass.h ../src/ode/collision_space.h ../src/common/OptionEntry.h ../src/GLEXT/GLState.h ../src/ode/timer.h ../src/tank/Tank.h ../src/tank/TankBatteries.h ../src/actions/ShotProjectile.h ../src/ode/collision.h ../src/ode/space.h ../src/weapons/Weapon.h ../src/weapons/Accessory.h ../src/ode/matrix.h ../src/engine/PhysicsEngine.h ../src/tank/TankAccessories.h ../src/ode/ode.h ../src/coms/NetBuffer.h ../src/weapons/Shield.h ../src/tank/TankModelId.h ../src/GLEXT/GLImage.h ../src/sprites/TracerActionRenderer.cpp ../src/tank/TankState.h ../src/common/ProgressCounter.h ../src/tank/TankContainer.h ../src/ode/geom.h ../src/ode/common.h ../src/actions/ShotProjectileTracer.h ../src/ode/objects.h ../src/ode/error.h ../src/engine/PhysicsParticleObject.h ../src/tank/TankFuel.h ../src/tank/TankParachutes.h ../src/ode/config.h ../src/common/ARGParser.h ../src/ode/odecpp.h ../src/tank/TankScore.h ../src/GLEXT/GLLenseFlare.h
../src/scorched/Release/srcGLWGLWidgetscorched.o: ../src/GLW/GLWidget.h ../src/GLW/GLWidget.cpp ../src/common/Defines.h ../src/GLEXT/GLState.h ../src/GLW/GLWPanel.h ../src/common/KeyboardHistory.h
../src/scorched/Release/srcGLEXTGLConsoleRulescorched.o: ../src/common/Defines.h ../src/GLEXT/GLConsoleRule.cpp ../src/GLEXT/GLConsoleRule.h
../src/scorched/Release/srccomsComsConnectAcceptMessagescorched.o: ../src/common/FileLines.h ../src/common/Defines.h ../src/common/OptionEntry.h ../src/coms/NetMessage.h ../src/coms/ComsConnectAcceptMessage.h ../src/coms/ComsMessage.h ../src/common/OptionsGame.h ../src/coms/NetBuffer.h ../src/coms/ComsConnectAcceptMessage.cpp ../src/common/ARGParser.h
