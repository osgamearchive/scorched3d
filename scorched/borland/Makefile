# Change these two lines to point to the location borland compiler
# and the dx sdk
BORLANDDIR = L:\Borland\BCC55
DXSDKDIR = L:\dx8\DXF\DXSDK


# General definitions
RM = del
TLIB = $(BORLANDDIR)\Bin\tlib.exe
ILINK = $(BORLANDDIR)\Bin\ilink32.exe
BRC32 = $(BORLANDDIR)\Bin\brc32.exe
TOUCH = $(BORLANDDIR)\Bin\touch.exe
LINKLIBS = import32.lib + cw32mt.lib + dxguid.lib + sdl.lib + sdlmain.lib + SDL_mixer.lib + SDL_net.lib
CC = $(BORLANDDIR)\Bin\bcc32.exe
INCLUDE = -I..\src -I$(DXSDKDIR)\include -I$(BORLANDDIR)\Include -I..\visualc\include
LIBS = -L$(BORLANDDIR)\lib -L$(BORLANDDIR)\dxlib -L$(BORLANDDIR)\sdllib
DEFINES = -DNO_FLOAT_MATH -DCOMPILED_FROM_DSP -D_NO_SERVER_ASE_ -D__MSC;INC_OLE2;__WIN95__;__WINDOWS__;WIN32;__BIDE__;dDOUBLE
DEFINESscorchedsvr = $(DEFINES) -DSCORCHED_SERVER
DEFINESscorched = $(DEFINES)
CCFLAGS = $(INCLUDE) -a8 -5 -tWM -W -w -w! -w-inl -w-8022 -w-8008 -w-8071 -w-8084 -w-8017 -w-8001 -w-8060 -w-rch -w-aus -w-par -v 
LINKFLAGS = -I..\release -x -Gn -Tpe -aa -c -v -V4.0

all : \
	client
client : \
	..\src\scorched\Release\scorchedb.exe

clean : \
	cleanscorched 
# Libraries for scorched
SCORCHEDODEOBJECTS = \
	..\src\scorched\release\srcodearrayscorched.obj \
	..\src\scorched\release\srcodeerrorscorched.obj \
	..\src\scorched\release\srcodefastdotscorched.obj \
	..\src\scorched\release\srcodefastldltscorched.obj \
	..\src\scorched\release\srcodefastlsolvescorched.obj \
	..\src\scorched\release\srcodefastltsolvescorched.obj \
	..\src\scorched\release\srcodegeomscorched.obj \
	..\src\scorched\release\srcodejointscorched.obj \
	..\src\scorched\release\srcodelcpscorched.obj \
	..\src\scorched\release\srcodemassscorched.obj \
	..\src\scorched\release\srcodematscorched.obj \
	..\src\scorched\release\srcodematrixscorched.obj \
	..\src\scorched\release\srcodememoryscorched.obj \
	..\src\scorched\release\srcodemiscscorched.obj \
	..\src\scorched\release\srcodeobstackscorched.obj \
	..\src\scorched\release\srcodeodescorched.obj \
	..\src\scorched\release\srcodeodemathscorched.obj \
	..\src\scorched\release\srcoderotationscorched.obj \
	..\src\scorched\release\srcodespacescorched.obj \
	..\src\scorched\release\srcodestepscorched.obj \
	..\src\scorched\release\srcodetestingscorched.obj

SCORCHEDTANKOBJECTS = \
	..\src\scorched\release\srctankTankscorched.obj \
	..\src\scorched\release\srctankTankAIscorched.obj \
	..\src\scorched\release\srctankTankAccessoriesscorched.obj \
	..\src\scorched\release\srctankTankAutoDefensescorched.obj \
	..\src\scorched\release\srctankTankBatteriesscorched.obj \
	..\src\scorched\release\srctankTankColorGeneratorscorched.obj \
	..\src\scorched\release\srctankTankContainerscorched.obj \
	..\src\scorched\release\srctankTankControllerscorched.obj \
	..\src\scorched\release\srctankTankLibscorched.obj \
	..\src\scorched\release\srctankTankModelIdscorched.obj \
	..\src\scorched\release\srctankTankParachutesscorched.obj \
	..\src\scorched\release\srctankTankPhysicsscorched.obj \
	..\src\scorched\release\srctankTankScorescorched.obj \
	..\src\scorched\release\srctankTankShieldsscorched.obj \
	..\src\scorched\release\srctankTankSortscorched.obj \
	..\src\scorched\release\srctankTankStartscorched.obj \
	..\src\scorched\release\srctankTankStatescorched.obj \
	..\src\scorched\release\srctankTankWeaponscorched.obj

SCORCHEDSCORCHEDOBJECTS = \
	..\src\scorched\release\srcscorchedCreateDialogsscorched.obj \
	..\src\scorched\release\srcscorchedDisplayDialogscorched.obj \
	..\src\scorched\release\srcscorchedMainDialogscorched.obj \
	..\src\scorched\release\srcscorchedNetDialogscorched.obj \
	..\src\scorched\release\srcscorchedmainscorched.obj

SCORCHEDDIALOGSOBJECTS = \
	..\src\scorched\release\srcdialogsBackdropDialogscorched.obj \
	..\src\scorched\release\srcdialogsBuyAccessoryDialogscorched.obj \
	..\src\scorched\release\srcdialogsConnectDialogscorched.obj \
	..\src\scorched\release\srcdialogsHelpDialogscorched.obj \
	..\src\scorched\release\srcdialogsKibitzingDialogscorched.obj \
	..\src\scorched\release\srcdialogsKillDialogscorched.obj \
	..\src\scorched\release\srcdialogsMainMenuDialogscorched.obj \
	..\src\scorched\release\srcdialogsOptionsDialogscorched.obj \
	..\src\scorched\release\srcdialogsPlanViewDialogscorched.obj \
	..\src\scorched\release\srcdialogsPlayerDialogscorched.obj \
	..\src\scorched\release\srcdialogsPlayersDialogscorched.obj \
	..\src\scorched\release\srcdialogsProgressDialogscorched.obj \
	..\src\scorched\release\srcdialogsQuitDialogscorched.obj \
	..\src\scorched\release\srcdialogsScoreDialogscorched.obj \
	..\src\scorched\release\srcdialogsTalkDialogscorched.obj \
	..\src\scorched\release\srcdialogsTankDialogscorched.obj \
	..\src\scorched\release\srcdialogsWindDialogscorched.obj

SCORCHEDZLIBOBJECTS = \
	..\src\scorched\release\srczlibadler32scorched.obj \
	..\src\scorched\release\srczlibcompressscorched.obj \
	..\src\scorched\release\srczlibcrc32scorched.obj \
	..\src\scorched\release\srczlibdeflatescorched.obj \
	..\src\scorched\release\srczlibgzioscorched.obj \
	..\src\scorched\release\srczlibinfblockscorched.obj \
	..\src\scorched\release\srczlibinfcodesscorched.obj \
	..\src\scorched\release\srczlibinffastscorched.obj \
	..\src\scorched\release\srczlibinflatescorched.obj \
	..\src\scorched\release\srczlibinftreesscorched.obj \
	..\src\scorched\release\srczlibinfutilscorched.obj \
	..\src\scorched\release\srczlibtreesscorched.obj \
	..\src\scorched\release\srczlibuncomprscorched.obj \
	..\src\scorched\release\srczlibzutilscorched.obj

SCORCHEDENGINEOBJECTS = \
	..\src\scorched\release\srcengineActionscorched.obj \
	..\src\scorched\release\srcengineActionBufferscorched.obj \
	..\src\scorched\release\srcengineActionControllerscorched.obj \
	..\src\scorched\release\srcengineActionMetascorched.obj \
	..\src\scorched\release\srcengineFrameTimerscorched.obj \
	..\src\scorched\release\srcengineGameStatescorched.obj \
	..\src\scorched\release\srcengineGameStateIscorched.obj \
	..\src\scorched\release\srcengineGameStateStimulusIscorched.obj \
	..\src\scorched\release\srcengineMainLoopscorched.obj \
	..\src\scorched\release\srcengineMainLoopIscorched.obj \
	..\src\scorched\release\srcenginePhysicsEnginescorched.obj \
	..\src\scorched\release\srcenginePhysicsParticlescorched.obj \
	..\src\scorched\release\srcenginePhysicsParticleObjectscorched.obj \
	..\src\scorched\release\srcengineScorchedCollisionHandlerscorched.obj \
	..\src\scorched\release\srcengineScorchedCollisionIdsscorched.obj

SCORCHEDGLWOBJECTS = \
	..\src\scorched\release\srcGLWGLWButtonscorched.obj \
	..\src\scorched\release\srcGLWGLWCheckBoxscorched.obj \
	..\src\scorched\release\srcGLWGLWDropDownscorched.obj \
	..\src\scorched\release\srcGLWGLWFileViewscorched.obj \
	..\src\scorched\release\srcGLWGLWFlagscorched.obj \
	..\src\scorched\release\srcGLWGLWFontscorched.obj \
	..\src\scorched\release\srcGLWGLWLabelscorched.obj \
	..\src\scorched\release\srcGLWGLWListViewscorched.obj \
	..\src\scorched\release\srcGLWGLWPanelscorched.obj \
	..\src\scorched\release\srcGLWGLWProgressscorched.obj \
	..\src\scorched\release\srcGLWGLWPushButtonscorched.obj \
	..\src\scorched\release\srcGLWGLWScrollButtonscorched.obj \
	..\src\scorched\release\srcGLWGLWScrollHscorched.obj \
	..\src\scorched\release\srcGLWGLWScrollPanelscorched.obj \
	..\src\scorched\release\srcGLWGLWScrollWscorched.obj \
	..\src\scorched\release\srcGLWGLWSpinBoxscorched.obj \
	..\src\scorched\release\srcGLWGLWTabscorched.obj \
	..\src\scorched\release\srcGLWGLWTextBoxscorched.obj \
	..\src\scorched\release\srcGLWGLWTextButtonscorched.obj \
	..\src\scorched\release\srcGLWGLWVisiblePanelscorched.obj \
	..\src\scorched\release\srcGLWGLWVisibleWidgetscorched.obj \
	..\src\scorched\release\srcGLWGLWWindowscorched.obj \
	..\src\scorched\release\srcGLWGLWidgetscorched.obj

SCORCHEDTANKGRAPHOBJECTS = \
	..\src\scorched\release\srctankgraphGLWTankViewerscorched.obj \
	..\src\scorched\release\srctankgraphTankMenusscorched.obj \
	..\src\scorched\release\srctankgraphTankMeshscorched.obj \
	..\src\scorched\release\srctankgraphTankModelscorched.obj \
	..\src\scorched\release\srctankgraphTankModelRendererscorched.obj \
	..\src\scorched\release\srctankgraphTankModelStorescorched.obj \
	..\src\scorched\release\srctankgraphTankRendererscorched.obj \
	..\src\scorched\release\srctankgraphTracerStorescorched.obj

SCORCHEDSPRITESOBJECTS = \
	..\src\scorched\release\srcspritesDebrisActionRendererscorched.obj \
	..\src\scorched\release\srcspritesExplosionRendererscorched.obj \
	..\src\scorched\release\srcspritesExplosionTexturesscorched.obj \
	..\src\scorched\release\srcspritesFlareActionRendererscorched.obj \
	..\src\scorched\release\srcspritesSmokeActionRendererscorched.obj \
	..\src\scorched\release\srcspritesTalkRendererscorched.obj \
	..\src\scorched\release\srcspritesTankDeadRendererscorched.obj \
	..\src\scorched\release\srcspritesTextActionRendererscorched.obj \
	..\src\scorched\release\srcspritesTracerActionRendererscorched.obj \
	..\src\scorched\release\srcspritesWallActionRendererscorched.obj

SCORCHEDACTIONSOBJECTS = \
	..\src\scorched\release\srcactionsExplosionscorched.obj \
	..\src\scorched\release\srcactionsShieldHitscorched.obj \
	..\src\scorched\release\srcactionsShotProjectilescorched.obj \
	..\src\scorched\release\srcactionsShotProjectileClodscorched.obj \
	..\src\scorched\release\srcactionsShotProjectileExplosionscorched.obj \
	..\src\scorched\release\srcactionsShotProjectileFunkyscorched.obj \
	..\src\scorched\release\srcactionsShotProjectileMirvscorched.obj \
	..\src\scorched\release\srcactionsShotProjectileRiotscorched.obj \
	..\src\scorched\release\srcactionsShotProjectileTracerscorched.obj \
	..\src\scorched\release\srcactionsSpriteProjectilescorched.obj \
	..\src\scorched\release\srcactionsTankDamagescorched.obj \
	..\src\scorched\release\srcactionsTankDeadscorched.obj \
	..\src\scorched\release\srcactionsTankFallingscorched.obj \
	..\src\scorched\release\srcactionsTankFiredscorched.obj \
	..\src\scorched\release\srcactionsTankMovescorched.obj \
	..\src\scorched\release\srcactionsTankResignscorched.obj \
	..\src\scorched\release\srcactionsTankSayscorched.obj \
	..\src\scorched\release\srcactionsTankScoredscorched.obj \
	..\src\scorched\release\srcactionsWallHitscorched.obj

SCORCHED3DSPARSEOBJECTS = \
	..\src\scorched\release\src3dsparseASEArrayFactscorched.obj \
	..\src\scorched\release\src3dsparseASEFilescorched.obj \
	..\src\scorched\release\src3dsparseASEModelscorched.obj \
	..\src\scorched\release\src3dsparseASEStorescorched.obj \
	..\src\scorched\release\src3dsparseMeshLODscorched.obj \
	..\src\scorched\release\src3dsparseMeshLODTriscorched.obj \
	..\src\scorched\release\src3dsparseMeshLODVectorscorched.obj \
	..\src\scorched\release\src3dsparseaseFile.lexscorched.obj \
	..\src\scorched\release\src3dsparseaseFile.tabscorched.obj

SCORCHEDTANKAIOBJECTS = \
	..\src\scorched\release\srctankaiTankAIComputerscorched.obj \
	..\src\scorched\release\srctankaiTankAIComputerBuyerscorched.obj \
	..\src\scorched\release\srctankaiTankAIComputerChooserscorched.obj \
	..\src\scorched\release\srctankaiTankAIComputerCtrlscorched.obj \
	..\src\scorched\release\srctankaiTankAIComputerDHTestscorched.obj \
	..\src\scorched\release\srctankaiTankAIComputerMoronscorched.obj \
	..\src\scorched\release\srctankaiTankAIComputerPSharkscorched.obj \
	..\src\scorched\release\srctankaiTankAIComputerRandomscorched.obj \
	..\src\scorched\release\srctankaiTankAIComputerShooterscorched.obj \
	..\src\scorched\release\srctankaiTankAIComputerTosserscorched.obj \
	..\src\scorched\release\srctankaiTankAIHumanscorched.obj \
	..\src\scorched\release\srctankaiTankAIHumanCtrlscorched.obj \
	..\src\scorched\release\srctankaiTankAIStorescorched.obj \
	..\src\scorched\release\srctankaiTankAIStringsscorched.obj

SCORCHEDCOMSOBJECTS = \
	..\src\scorched\release\srccomsComsActionsMessagescorched.obj \
	..\src\scorched\release\srccomsComsAddPlayerMessagescorched.obj \
	..\src\scorched\release\srccomsComsBuyAccessoryMessagescorched.obj \
	..\src\scorched\release\srccomsComsConnectAcceptMessagescorched.obj \
	..\src\scorched\release\srccomsComsConnectMessagescorched.obj \
	..\src\scorched\release\srccomsComsDefenseMessagescorched.obj \
	..\src\scorched\release\srccomsComsGameStoppedMessagescorched.obj \
	..\src\scorched\release\srccomsComsLevelMessagescorched.obj \
	..\src\scorched\release\srccomsComsMessagescorched.obj \
	..\src\scorched\release\srccomsComsMessageHandlerscorched.obj \
	..\src\scorched\release\srccomsComsMessageSenderscorched.obj \
	..\src\scorched\release\srccomsComsNewGameMessagescorched.obj \
	..\src\scorched\release\srccomsComsPlayedMoveMessagescorched.obj \
	..\src\scorched\release\srccomsComsPlayerReadyMessagescorched.obj \
	..\src\scorched\release\srccomsComsPlayerStateMessagescorched.obj \
	..\src\scorched\release\srccomsComsRmPlayerMessagescorched.obj \
	..\src\scorched\release\srccomsComsScoreMessagescorched.obj \
	..\src\scorched\release\srccomsComsStartGameMessagescorched.obj \
	..\src\scorched\release\srccomsComsTextMessagescorched.obj \
	..\src\scorched\release\srccomsNetBufferscorched.obj \
	..\src\scorched\release\srccomsNetBufferUtilscorched.obj \
	..\src\scorched\release\srccomsNetClientscorched.obj \
	..\src\scorched\release\srccomsNetMessagescorched.obj \
	..\src\scorched\release\srccomsNetMessageHandlerscorched.obj \
	..\src\scorched\release\srccomsNetMessagePoolscorched.obj \
	..\src\scorched\release\srccomsNetServerscorched.obj

SCORCHEDWEAPONSOBJECTS = \
	..\src\scorched\release\srcweaponsAccessoryscorched.obj \
	..\src\scorched\release\srcweaponsAccessoryStorescorched.obj \
	..\src\scorched\release\srcweaponsAutoDefensescorched.obj \
	..\src\scorched\release\srcweaponsBatteryscorched.obj \
	..\src\scorched\release\srcweaponsParachutescorched.obj \
	..\src\scorched\release\srcweaponsShieldscorched.obj \
	..\src\scorched\release\srcweaponsShieldReflectivescorched.obj \
	..\src\scorched\release\srcweaponsShieldReflectiveMagscorched.obj \
	..\src\scorched\release\srcweaponsWeaponscorched.obj \
	..\src\scorched\release\srcweaponsWeaponClodscorched.obj \
	..\src\scorched\release\srcweaponsWeaponFunkyscorched.obj \
	..\src\scorched\release\srcweaponsWeaponMirvscorched.obj \
	..\src\scorched\release\srcweaponsWeaponProjectilescorched.obj \
	..\src\scorched\release\srcweaponsWeaponRiotBombscorched.obj \
	..\src\scorched\release\srcweaponsWeaponTracerscorched.obj

SCORCHEDGLEXTOBJECTS = \
	..\src\scorched\release\srcGLEXTGLBitmapscorched.obj \
	..\src\scorched\release\srcGLEXTGLBitmapItteratorscorched.obj \
	..\src\scorched\release\srcGLEXTGLBitmapModifierscorched.obj \
	..\src\scorched\release\srcGLEXTGLCamerascorched.obj \
	..\src\scorched\release\srcGLEXTGLCameraFrustumscorched.obj \
	..\src\scorched\release\srcGLEXTGLConsolescorched.obj \
	..\src\scorched\release\srcGLEXTGLConsoleFileReaderscorched.obj \
	..\src\scorched\release\srcGLEXTGLConsoleLinesscorched.obj \
	..\src\scorched\release\srcGLEXTGLConsoleMethodsscorched.obj \
	..\src\scorched\release\srcGLEXTGLConsoleRulescorched.obj \
	..\src\scorched\release\srcGLEXTGLConsoleRuleFnscorched.obj \
	..\src\scorched\release\srcGLEXTGLConsoleRuleFnIAdapterscorched.obj \
	..\src\scorched\release\srcGLEXTGLConsoleRuleMethodscorched.obj \
	..\src\scorched\release\srcGLEXTGLConsoleRulesscorched.obj \
	..\src\scorched\release\srcGLEXTGLFont2dscorched.obj \
	..\src\scorched\release\srcGLEXTGLFontBannerscorched.obj \
	..\src\scorched\release\srcGLEXTGLImagescorched.obj \
	..\src\scorched\release\srcGLEXTGLLenseFlarescorched.obj \
	..\src\scorched\release\srcGLEXTGLLuminancescorched.obj \
	..\src\scorched\release\srcGLEXTGLMenuscorched.obj \
	..\src\scorched\release\srcGLEXTGLMenuEntryscorched.obj \
	..\src\scorched\release\srcGLEXTGLMenuIscorched.obj \
	..\src\scorched\release\srcGLEXTGLStatescorched.obj \
	..\src\scorched\release\srcGLEXTGLStateExtensionscorched.obj \
	..\src\scorched\release\srcGLEXTGLTexturescorched.obj \
	..\src\scorched\release\srcGLEXTGLTextureCubeMapscorched.obj \
	..\src\scorched\release\srcGLEXTGLTextureSetscorched.obj \
	..\src\scorched\release\srcGLEXTGLVarscorched.obj \
	..\src\scorched\release\srcGLEXTGLVertexArrayscorched.obj \
	..\src\scorched\release\srcGLEXTGLVertexTexArrayscorched.obj

SCORCHEDCOMMONOBJECTS = \
	..\src\scorched\release\srccommonARGParserscorched.obj \
	..\src\scorched\release\srccommonDefinesscorched.obj \
	..\src\scorched\release\srccommonDisplayscorched.obj \
	..\src\scorched\release\srccommonFacescorched.obj \
	..\src\scorched\release\srccommonFileLinesscorched.obj \
	..\src\scorched\release\srccommonFileListscorched.obj \
	..\src\scorched\release\srccommonGammascorched.obj \
	..\src\scorched\release\srccommonKeyboardscorched.obj \
	..\src\scorched\release\srccommonLinescorched.obj \
	..\src\scorched\release\srccommonMousescorched.obj \
	..\src\scorched\release\srccommonOptionEntryscorched.obj \
	..\src\scorched\release\srccommonOptionsGamescorched.obj \
	..\src\scorched\release\srccommonOptionsTransientscorched.obj \
	..\src\scorched\release\srccommonProgressCounterscorched.obj \
	..\src\scorched\release\srccommonRandomGeneratorscorched.obj \
	..\src\scorched\release\srccommonRegistryscorched.obj \
	..\src\scorched\release\srccommonSoundscorched.obj \
	..\src\scorched\release\srccommonSoundBufferscorched.obj \
	..\src\scorched\release\srccommonSoundStorescorched.obj \
	..\src\scorched\release\srccommonTimerscorched.obj \
	..\src\scorched\release\srccommonTrianglescorched.obj \
	..\src\scorched\release\srccommonVectorscorched.obj \
	..\src\scorched\release\srccommonVectorLibscorched.obj \
	..\src\scorched\release\srccommonWindowManagerscorched.obj

SCORCHEDLANDSCAPEOBJECTS = \
	..\src\scorched\release\srclandscapeCloudSimscorched.obj \
	..\src\scorched\release\srclandscapeDeformLandscapescorched.obj \
	..\src\scorched\release\srclandscapeDeformTexturesscorched.obj \
	..\src\scorched\release\srclandscapeGlobalHMapscorched.obj \
	..\src\scorched\release\srclandscapeHeightMapscorched.obj \
	..\src\scorched\release\srclandscapeHeightMapCollisionscorched.obj \
	..\src\scorched\release\srclandscapeHeightMapLoaderscorched.obj \
	..\src\scorched\release\srclandscapeHeightMapModifierscorched.obj \
	..\src\scorched\release\srclandscapeHeightMapRendererscorched.obj \
	..\src\scorched\release\srclandscapeHeightMapSurroundscorched.obj \
	..\src\scorched\release\srclandscapeHemispherescorched.obj \
	..\src\scorched\release\srclandscapeLanscapescorched.obj \
	..\src\scorched\release\srclandscapePatchscorched.obj \
	..\src\scorched\release\srclandscapePatchGridscorched.obj \
	..\src\scorched\release\srclandscapePatchSidescorched.obj \
	..\src\scorched\release\srclandscapePatchVarscorched.obj \
	..\src\scorched\release\srclandscapeShadowMapscorched.obj \
	..\src\scorched\release\srclandscapeSmokescorched.obj \
	..\src\scorched\release\srclandscapeSmokeChainscorched.obj \
	..\src\scorched\release\srclandscapeSunscorched.obj \
	..\src\scorched\release\srclandscapeSurroundscorched.obj \
	..\src\scorched\release\srclandscapeSurroundDefsscorched.obj \
	..\src\scorched\release\srclandscapeTriNodescorched.obj \
	..\src\scorched\release\srclandscapeTriNodePoolscorched.obj \
	..\src\scorched\release\srclandscapeVariancescorched.obj \
	..\src\scorched\release\srclandscapeWallscorched.obj \
	..\src\scorched\release\srclandscapeWaterMapscorched.obj \
	..\src\scorched\release\srclandscapeWaterMapModifierscorched.obj \
	..\src\scorched\release\srclandscapeWaterMapPointsscorched.obj \
	..\src\scorched\release\srclandscapeWaterMapSurroundscorched.obj

SCORCHEDXMLOBJECTS = \
	..\src\scorched\release\srcXMLXMLFilescorched.obj \
	..\src\scorched\release\srcXMLXMLParserscorched.obj \
	..\src\scorched\release\srcXMLxmlparsescorched.obj \
	..\src\scorched\release\srcXMLxmlrolescorched.obj \
	..\src\scorched\release\srcXMLxmltokscorched.obj

SCORCHEDCLIENTOBJECTS = \
	..\src\scorched\release\srcclientClientActionsHandlerscorched.obj \
	..\src\scorched\release\srcclientClientAddPlayerHandlerscorched.obj \
	..\src\scorched\release\srcclientClientBuyAccessoryHandlerscorched.obj \
	..\src\scorched\release\srcclientClientConnectionAcceptHandlerscorched.obj \
	..\src\scorched\release\srcclientClientDefenseHandlerscorched.obj \
	..\src\scorched\release\srcclientClientDialogscorched.obj \
	..\src\scorched\release\srcclientClientGameStoppedHandlerscorched.obj \
	..\src\scorched\release\srcclientClientMainscorched.obj \
	..\src\scorched\release\srcclientClientMessageHandlerscorched.obj \
	..\src\scorched\release\srcclientClientNewGameHandlerscorched.obj \
	..\src\scorched\release\srcclientClientNextRoundStatescorched.obj \
	..\src\scorched\release\srcclientClientRmPlayerHandlerscorched.obj \
	..\src\scorched\release\srcclientClientScoreHandlerscorched.obj \
	..\src\scorched\release\srcclientClientShotStatescorched.obj \
	..\src\scorched\release\srcclientClientStartGameHandlerscorched.obj \
	..\src\scorched\release\srcclientClientStatescorched.obj \
	..\src\scorched\release\srcclientClientTextHandlerscorched.obj \
	..\src\scorched\release\srcclientGLSetupscorched.obj \
	..\src\scorched\release\srcclientMain2DCamerascorched.obj \
	..\src\scorched\release\srcclientMainBannerscorched.obj \
	..\src\scorched\release\srcclientMainCamerascorched.obj \
	..\src\scorched\release\srcclientOptionsClientscorched.obj \
	..\src\scorched\release\srcclientShotTimerscorched.obj \
	..\src\scorched\release\srcclientSpeedChangescorched.obj \
	..\src\scorched\release\srcclientWindowSetupscorched.obj

SCORCHEDSERVEROBJECTS = \
	..\src\scorched\release\srcserverOptionsServerscorched.obj \
	..\src\scorched\release\srcserverScorchedASEscorched.obj \
	..\src\scorched\release\srcserverServerBuyAccessoryHandlerscorched.obj \
	..\src\scorched\release\srcserverServerConnectHandlerscorched.obj \
	..\src\scorched\release\srcserverServerDefenseHandlerscorched.obj \
	..\src\scorched\release\srcserverServerDialogscorched.obj \
	..\src\scorched\release\srcserverServerMainscorched.obj \
	..\src\scorched\release\srcserverServerMessageHandlerscorched.obj \
	..\src\scorched\release\srcserverServerNewGameStatescorched.obj \
	..\src\scorched\release\srcserverServerNextRoundStatescorched.obj \
	..\src\scorched\release\srcserverServerPlayedMoveHandlerscorched.obj \
	..\src\scorched\release\srcserverServerPlayerReadyHandlerscorched.obj \
	..\src\scorched\release\srcserverServerPlayingStatescorched.obj \
	..\src\scorched\release\srcserverServerScoreStatescorched.obj \
	..\src\scorched\release\srcserverServerShotHolderscorched.obj \
	..\src\scorched\release\srcserverServerShotStatescorched.obj \
	..\src\scorched\release\srcserverServerStatescorched.obj \
	..\src\scorched\release\srcserverServerTextHandlerscorched.obj \
	..\src\scorched\release\srcserverServerTooFewPlayersStimulusscorched.obj \
	..\src\scorched\release\srcserverServerWaitingForPlayersStatescorched.obj


..\src\scorched\release\scorchedb.exe : \
	..\src\scorched\release\SCORCHEDODE.lib \ 
	..\src\scorched\release\SCORCHEDTANK.lib \ 
	..\src\scorched\release\SCORCHEDSCORCHED.lib \ 
	..\src\scorched\release\SCORCHEDDIALOGS.lib \ 
	..\src\scorched\release\SCORCHEDZLIB.lib \ 
	..\src\scorched\release\SCORCHEDENGINE.lib \ 
	..\src\scorched\release\SCORCHEDGLW.lib \ 
	..\src\scorched\release\SCORCHEDTANKGRAPH.lib \ 
	..\src\scorched\release\SCORCHEDSPRITES.lib \ 
	..\src\scorched\release\SCORCHEDACTIONS.lib \ 
	..\src\scorched\release\SCORCHED3DSPARSE.lib \ 
	..\src\scorched\release\SCORCHEDTANKAI.lib \ 
	..\src\scorched\release\SCORCHEDCOMS.lib \ 
	..\src\scorched\release\SCORCHEDWEAPONS.lib \ 
	..\src\scorched\release\SCORCHEDGLEXT.lib \ 
	..\src\scorched\release\SCORCHEDCOMMON.lib \ 
	..\src\scorched\release\SCORCHEDLANDSCAPE.lib \ 
	..\src\scorched\release\SCORCHEDXML.lib \ 
	..\src\scorched\release\SCORCHEDCLIENT.lib \ 
	..\src\scorched\release\SCORCHEDSERVER.lib 
	$(ILINK) $(LIBS) $(LINKFLAGS) c0w32.obj, ..\src\scorched\release\scorchedb.exe,, \
	$(LINKLIBS) + \
	..\src\scorched\release\SCORCHEDODE.lib + \
	..\src\scorched\release\SCORCHEDTANK.lib + \
	..\src\scorched\release\SCORCHEDSCORCHED.lib + \
	..\src\scorched\release\SCORCHEDDIALOGS.lib + \
	..\src\scorched\release\SCORCHEDZLIB.lib + \
	..\src\scorched\release\SCORCHEDENGINE.lib + \
	..\src\scorched\release\SCORCHEDGLW.lib + \
	..\src\scorched\release\SCORCHEDTANKGRAPH.lib + \
	..\src\scorched\release\SCORCHEDSPRITES.lib + \
	..\src\scorched\release\SCORCHEDACTIONS.lib + \
	..\src\scorched\release\SCORCHED3DSPARSE.lib + \
	..\src\scorched\release\SCORCHEDTANKAI.lib + \
	..\src\scorched\release\SCORCHEDCOMS.lib + \
	..\src\scorched\release\SCORCHEDWEAPONS.lib + \
	..\src\scorched\release\SCORCHEDGLEXT.lib + \
	..\src\scorched\release\SCORCHEDCOMMON.lib + \
	..\src\scorched\release\SCORCHEDLANDSCAPE.lib + \
	..\src\scorched\release\SCORCHEDXML.lib + \
	..\src\scorched\release\SCORCHEDCLIENT.lib + \
	..\src\scorched\release\SCORCHEDSERVER.lib \
	, ,

..\src\scorched\release\SCORCHEDODE.lib : $(SCORCHEDODEOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDODE.lib
	$(RM) ..\src\scorched\release\SCORCHEDODE.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDODE.lib /a $(SCORCHEDODEOBJECTS) 

..\src\scorched\release\SCORCHEDTANK.lib : $(SCORCHEDTANKOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDTANK.lib
	$(RM) ..\src\scorched\release\SCORCHEDTANK.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDTANK.lib /a $(SCORCHEDTANKOBJECTS) 

..\src\scorched\release\SCORCHEDSCORCHED.lib : $(SCORCHEDSCORCHEDOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDSCORCHED.lib
	$(RM) ..\src\scorched\release\SCORCHEDSCORCHED.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDSCORCHED.lib /a $(SCORCHEDSCORCHEDOBJECTS) 

..\src\scorched\release\SCORCHEDDIALOGS.lib : $(SCORCHEDDIALOGSOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDDIALOGS.lib
	$(RM) ..\src\scorched\release\SCORCHEDDIALOGS.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDDIALOGS.lib /a $(SCORCHEDDIALOGSOBJECTS) 

..\src\scorched\release\SCORCHEDZLIB.lib : $(SCORCHEDZLIBOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDZLIB.lib
	$(RM) ..\src\scorched\release\SCORCHEDZLIB.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDZLIB.lib /a $(SCORCHEDZLIBOBJECTS) 

..\src\scorched\release\SCORCHEDENGINE.lib : $(SCORCHEDENGINEOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDENGINE.lib
	$(RM) ..\src\scorched\release\SCORCHEDENGINE.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDENGINE.lib /a $(SCORCHEDENGINEOBJECTS) 

..\src\scorched\release\SCORCHEDGLW.lib : $(SCORCHEDGLWOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDGLW.lib
	$(RM) ..\src\scorched\release\SCORCHEDGLW.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDGLW.lib /a $(SCORCHEDGLWOBJECTS) 

..\src\scorched\release\SCORCHEDTANKGRAPH.lib : $(SCORCHEDTANKGRAPHOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDTANKGRAPH.lib
	$(RM) ..\src\scorched\release\SCORCHEDTANKGRAPH.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDTANKGRAPH.lib /a $(SCORCHEDTANKGRAPHOBJECTS) 

..\src\scorched\release\SCORCHEDSPRITES.lib : $(SCORCHEDSPRITESOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDSPRITES.lib
	$(RM) ..\src\scorched\release\SCORCHEDSPRITES.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDSPRITES.lib /a $(SCORCHEDSPRITESOBJECTS) 

..\src\scorched\release\SCORCHEDACTIONS.lib : $(SCORCHEDACTIONSOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDACTIONS.lib
	$(RM) ..\src\scorched\release\SCORCHEDACTIONS.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDACTIONS.lib /a $(SCORCHEDACTIONSOBJECTS) 

..\src\scorched\release\SCORCHED3DSPARSE.lib : $(SCORCHED3DSPARSEOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHED3DSPARSE.lib
	$(RM) ..\src\scorched\release\SCORCHED3DSPARSE.lib
	$(TLIB) ..\src\scorched\release\SCORCHED3DSPARSE.lib /a $(SCORCHED3DSPARSEOBJECTS) 

..\src\scorched\release\SCORCHEDTANKAI.lib : $(SCORCHEDTANKAIOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDTANKAI.lib
	$(RM) ..\src\scorched\release\SCORCHEDTANKAI.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDTANKAI.lib /a $(SCORCHEDTANKAIOBJECTS) 

..\src\scorched\release\SCORCHEDCOMS.lib : $(SCORCHEDCOMSOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDCOMS.lib
	$(RM) ..\src\scorched\release\SCORCHEDCOMS.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDCOMS.lib /a $(SCORCHEDCOMSOBJECTS) 

..\src\scorched\release\SCORCHEDWEAPONS.lib : $(SCORCHEDWEAPONSOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDWEAPONS.lib
	$(RM) ..\src\scorched\release\SCORCHEDWEAPONS.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDWEAPONS.lib /a $(SCORCHEDWEAPONSOBJECTS) 

..\src\scorched\release\SCORCHEDGLEXT.lib : $(SCORCHEDGLEXTOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDGLEXT.lib
	$(RM) ..\src\scorched\release\SCORCHEDGLEXT.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDGLEXT.lib /a $(SCORCHEDGLEXTOBJECTS) 

..\src\scorched\release\SCORCHEDCOMMON.lib : $(SCORCHEDCOMMONOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDCOMMON.lib
	$(RM) ..\src\scorched\release\SCORCHEDCOMMON.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDCOMMON.lib /a $(SCORCHEDCOMMONOBJECTS) 

..\src\scorched\release\SCORCHEDLANDSCAPE.lib : $(SCORCHEDLANDSCAPEOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDLANDSCAPE.lib
	$(RM) ..\src\scorched\release\SCORCHEDLANDSCAPE.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDLANDSCAPE.lib /a $(SCORCHEDLANDSCAPEOBJECTS) 

..\src\scorched\release\SCORCHEDXML.lib : $(SCORCHEDXMLOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDXML.lib
	$(RM) ..\src\scorched\release\SCORCHEDXML.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDXML.lib /a $(SCORCHEDXMLOBJECTS) 

..\src\scorched\release\SCORCHEDCLIENT.lib : $(SCORCHEDCLIENTOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDCLIENT.lib
	$(RM) ..\src\scorched\release\SCORCHEDCLIENT.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDCLIENT.lib /a $(SCORCHEDCLIENTOBJECTS) 

..\src\scorched\release\SCORCHEDSERVER.lib : $(SCORCHEDSERVEROBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDSERVER.lib
	$(RM) ..\src\scorched\release\SCORCHEDSERVER.lib
	$(TLIB) ..\src\scorched\release\SCORCHEDSERVER.lib /a $(SCORCHEDSERVEROBJECTS) 

cleanscorched :
	$(TOUCH) ..\src\scorched\release\scorchedb.exe
	$(RM) ..\src\scorched\release\scorchedb.*
	$(TOUCH) $(SCORCHEDODEOBJECTS)
	$(RM) $(SCORCHEDODEOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDODE.lib
	$(RM) ..\src\scorched\release\SCORCHEDODE.lib
	$(TOUCH) $(SCORCHEDTANKOBJECTS)
	$(RM) $(SCORCHEDTANKOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDTANK.lib
	$(RM) ..\src\scorched\release\SCORCHEDTANK.lib
	$(TOUCH) $(SCORCHEDSCORCHEDOBJECTS)
	$(RM) $(SCORCHEDSCORCHEDOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDSCORCHED.lib
	$(RM) ..\src\scorched\release\SCORCHEDSCORCHED.lib
	$(TOUCH) $(SCORCHEDDIALOGSOBJECTS)
	$(RM) $(SCORCHEDDIALOGSOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDDIALOGS.lib
	$(RM) ..\src\scorched\release\SCORCHEDDIALOGS.lib
	$(TOUCH) $(SCORCHEDZLIBOBJECTS)
	$(RM) $(SCORCHEDZLIBOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDZLIB.lib
	$(RM) ..\src\scorched\release\SCORCHEDZLIB.lib
	$(TOUCH) $(SCORCHEDENGINEOBJECTS)
	$(RM) $(SCORCHEDENGINEOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDENGINE.lib
	$(RM) ..\src\scorched\release\SCORCHEDENGINE.lib
	$(TOUCH) $(SCORCHEDGLWOBJECTS)
	$(RM) $(SCORCHEDGLWOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDGLW.lib
	$(RM) ..\src\scorched\release\SCORCHEDGLW.lib
	$(TOUCH) $(SCORCHEDTANKGRAPHOBJECTS)
	$(RM) $(SCORCHEDTANKGRAPHOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDTANKGRAPH.lib
	$(RM) ..\src\scorched\release\SCORCHEDTANKGRAPH.lib
	$(TOUCH) $(SCORCHEDSPRITESOBJECTS)
	$(RM) $(SCORCHEDSPRITESOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDSPRITES.lib
	$(RM) ..\src\scorched\release\SCORCHEDSPRITES.lib
	$(TOUCH) $(SCORCHEDACTIONSOBJECTS)
	$(RM) $(SCORCHEDACTIONSOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDACTIONS.lib
	$(RM) ..\src\scorched\release\SCORCHEDACTIONS.lib
	$(TOUCH) $(SCORCHED3DSPARSEOBJECTS)
	$(RM) $(SCORCHED3DSPARSEOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHED3DSPARSE.lib
	$(RM) ..\src\scorched\release\SCORCHED3DSPARSE.lib
	$(TOUCH) $(SCORCHEDTANKAIOBJECTS)
	$(RM) $(SCORCHEDTANKAIOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDTANKAI.lib
	$(RM) ..\src\scorched\release\SCORCHEDTANKAI.lib
	$(TOUCH) $(SCORCHEDCOMSOBJECTS)
	$(RM) $(SCORCHEDCOMSOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDCOMS.lib
	$(RM) ..\src\scorched\release\SCORCHEDCOMS.lib
	$(TOUCH) $(SCORCHEDWEAPONSOBJECTS)
	$(RM) $(SCORCHEDWEAPONSOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDWEAPONS.lib
	$(RM) ..\src\scorched\release\SCORCHEDWEAPONS.lib
	$(TOUCH) $(SCORCHEDGLEXTOBJECTS)
	$(RM) $(SCORCHEDGLEXTOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDGLEXT.lib
	$(RM) ..\src\scorched\release\SCORCHEDGLEXT.lib
	$(TOUCH) $(SCORCHEDCOMMONOBJECTS)
	$(RM) $(SCORCHEDCOMMONOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDCOMMON.lib
	$(RM) ..\src\scorched\release\SCORCHEDCOMMON.lib
	$(TOUCH) $(SCORCHEDLANDSCAPEOBJECTS)
	$(RM) $(SCORCHEDLANDSCAPEOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDLANDSCAPE.lib
	$(RM) ..\src\scorched\release\SCORCHEDLANDSCAPE.lib
	$(TOUCH) $(SCORCHEDXMLOBJECTS)
	$(RM) $(SCORCHEDXMLOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDXML.lib
	$(RM) ..\src\scorched\release\SCORCHEDXML.lib
	$(TOUCH) $(SCORCHEDCLIENTOBJECTS)
	$(RM) $(SCORCHEDCLIENTOBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDCLIENT.lib
	$(RM) ..\src\scorched\release\SCORCHEDCLIENT.lib
	$(TOUCH) $(SCORCHEDSERVEROBJECTS)
	$(RM) $(SCORCHEDSERVEROBJECTS)
	$(TOUCH) ..\src\scorched\release\SCORCHEDSERVER.lib
	$(RM) ..\src\scorched\release\SCORCHEDSERVER.lib

# CPP Build Rules
..\src\scorched\release\srcodearrayscorched.obj : ..\src\ode\array.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodearrayscorched.obj ..\src\ode\array.cpp

..\src\scorched\release\srcodeerrorscorched.obj : ..\src\ode\error.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodeerrorscorched.obj ..\src\ode\error.cpp

..\src\scorched\release\srcodefastdotscorched.obj : ..\src\ode\fastdot.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodefastdotscorched.obj ..\src\ode\fastdot.c

..\src\scorched\release\srcodefastldltscorched.obj : ..\src\ode\fastldlt.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodefastldltscorched.obj ..\src\ode\fastldlt.c

..\src\scorched\release\srcodefastlsolvescorched.obj : ..\src\ode\fastlsolve.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodefastlsolvescorched.obj ..\src\ode\fastlsolve.c

..\src\scorched\release\srcodefastltsolvescorched.obj : ..\src\ode\fastltsolve.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodefastltsolvescorched.obj ..\src\ode\fastltsolve.c

..\src\scorched\release\srcodegeomscorched.obj : ..\src\ode\geom.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodegeomscorched.obj ..\src\ode\geom.cpp

..\src\scorched\release\srcodejointscorched.obj : ..\src\ode\joint.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodejointscorched.obj ..\src\ode\joint.cpp

..\src\scorched\release\srcodelcpscorched.obj : ..\src\ode\lcp.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodelcpscorched.obj ..\src\ode\lcp.cpp

..\src\scorched\release\srcodemassscorched.obj : ..\src\ode\mass.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodemassscorched.obj ..\src\ode\mass.cpp

..\src\scorched\release\srcodematscorched.obj : ..\src\ode\mat.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodematscorched.obj ..\src\ode\mat.cpp

..\src\scorched\release\srcodematrixscorched.obj : ..\src\ode\matrix.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodematrixscorched.obj ..\src\ode\matrix.cpp

..\src\scorched\release\srcodememoryscorched.obj : ..\src\ode\memory.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodememoryscorched.obj ..\src\ode\memory.cpp

..\src\scorched\release\srcodemiscscorched.obj : ..\src\ode\misc.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodemiscscorched.obj ..\src\ode\misc.cpp

..\src\scorched\release\srcodeobstackscorched.obj : ..\src\ode\obstack.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodeobstackscorched.obj ..\src\ode\obstack.cpp

..\src\scorched\release\srcodeodescorched.obj : ..\src\ode\ode.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodeodescorched.obj ..\src\ode\ode.cpp

..\src\scorched\release\srcodeodemathscorched.obj : ..\src\ode\odemath.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodeodemathscorched.obj ..\src\ode\odemath.cpp

..\src\scorched\release\srcoderotationscorched.obj : ..\src\ode\rotation.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcoderotationscorched.obj ..\src\ode\rotation.cpp

..\src\scorched\release\srcodespacescorched.obj : ..\src\ode\space.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodespacescorched.obj ..\src\ode\space.cpp

..\src\scorched\release\srcodestepscorched.obj : ..\src\ode\step.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodestepscorched.obj ..\src\ode\step.cpp

..\src\scorched\release\srcodetestingscorched.obj : ..\src\ode\testing.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcodetestingscorched.obj ..\src\ode\testing.cpp

..\src\scorched\release\srctankTankscorched.obj : ..\src\tank\Tank.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankscorched.obj ..\src\tank\Tank.cpp

..\src\scorched\release\srctankTankAIscorched.obj : ..\src\tank\TankAI.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankAIscorched.obj ..\src\tank\TankAI.cpp

..\src\scorched\release\srctankTankAccessoriesscorched.obj : ..\src\tank\TankAccessories.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankAccessoriesscorched.obj ..\src\tank\TankAccessories.cpp

..\src\scorched\release\srctankTankAutoDefensescorched.obj : ..\src\tank\TankAutoDefense.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankAutoDefensescorched.obj ..\src\tank\TankAutoDefense.cpp

..\src\scorched\release\srctankTankBatteriesscorched.obj : ..\src\tank\TankBatteries.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankBatteriesscorched.obj ..\src\tank\TankBatteries.cpp

..\src\scorched\release\srctankTankColorGeneratorscorched.obj : ..\src\tank\TankColorGenerator.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankColorGeneratorscorched.obj ..\src\tank\TankColorGenerator.cpp

..\src\scorched\release\srctankTankContainerscorched.obj : ..\src\tank\TankContainer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankContainerscorched.obj ..\src\tank\TankContainer.cpp

..\src\scorched\release\srctankTankControllerscorched.obj : ..\src\tank\TankController.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankControllerscorched.obj ..\src\tank\TankController.cpp

..\src\scorched\release\srctankTankLibscorched.obj : ..\src\tank\TankLib.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankLibscorched.obj ..\src\tank\TankLib.cpp

..\src\scorched\release\srctankTankModelIdscorched.obj : ..\src\tank\TankModelId.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankModelIdscorched.obj ..\src\tank\TankModelId.cpp

..\src\scorched\release\srctankTankParachutesscorched.obj : ..\src\tank\TankParachutes.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankParachutesscorched.obj ..\src\tank\TankParachutes.cpp

..\src\scorched\release\srctankTankPhysicsscorched.obj : ..\src\tank\TankPhysics.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankPhysicsscorched.obj ..\src\tank\TankPhysics.cpp

..\src\scorched\release\srctankTankScorescorched.obj : ..\src\tank\TankScore.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankScorescorched.obj ..\src\tank\TankScore.cpp

..\src\scorched\release\srctankTankShieldsscorched.obj : ..\src\tank\TankShields.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankShieldsscorched.obj ..\src\tank\TankShields.cpp

..\src\scorched\release\srctankTankSortscorched.obj : ..\src\tank\TankSort.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankSortscorched.obj ..\src\tank\TankSort.cpp

..\src\scorched\release\srctankTankStartscorched.obj : ..\src\tank\TankStart.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankStartscorched.obj ..\src\tank\TankStart.cpp

..\src\scorched\release\srctankTankStatescorched.obj : ..\src\tank\TankState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankStatescorched.obj ..\src\tank\TankState.cpp

..\src\scorched\release\srctankTankWeaponscorched.obj : ..\src\tank\TankWeapon.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankTankWeaponscorched.obj ..\src\tank\TankWeapon.cpp

..\src\scorched\release\srcscorchedCreateDialogsscorched.obj : ..\src\scorched\CreateDialogs.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcscorchedCreateDialogsscorched.obj ..\src\scorched\CreateDialogs.cpp

..\src\scorched\release\srcscorchedDisplayDialogscorched.obj : ..\src\scorched\DisplayDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcscorchedDisplayDialogscorched.obj ..\src\scorched\DisplayDialog.cpp

..\src\scorched\release\srcscorchedMainDialogscorched.obj : ..\src\scorched\MainDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcscorchedMainDialogscorched.obj ..\src\scorched\MainDialog.cpp

..\src\scorched\release\srcscorchedNetDialogscorched.obj : ..\src\scorched\NetDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcscorchedNetDialogscorched.obj ..\src\scorched\NetDialog.cpp

..\src\scorched\release\srcscorchedmainscorched.obj : ..\src\scorched\main.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcscorchedmainscorched.obj ..\src\scorched\main.cpp

..\src\scorched\release\srcdialogsBackdropDialogscorched.obj : ..\src\dialogs\BackdropDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsBackdropDialogscorched.obj ..\src\dialogs\BackdropDialog.cpp

..\src\scorched\release\srcdialogsBuyAccessoryDialogscorched.obj : ..\src\dialogs\BuyAccessoryDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsBuyAccessoryDialogscorched.obj ..\src\dialogs\BuyAccessoryDialog.cpp

..\src\scorched\release\srcdialogsConnectDialogscorched.obj : ..\src\dialogs\ConnectDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsConnectDialogscorched.obj ..\src\dialogs\ConnectDialog.cpp

..\src\scorched\release\srcdialogsHelpDialogscorched.obj : ..\src\dialogs\HelpDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsHelpDialogscorched.obj ..\src\dialogs\HelpDialog.cpp

..\src\scorched\release\srcdialogsKibitzingDialogscorched.obj : ..\src\dialogs\KibitzingDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsKibitzingDialogscorched.obj ..\src\dialogs\KibitzingDialog.cpp

..\src\scorched\release\srcdialogsKillDialogscorched.obj : ..\src\dialogs\KillDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsKillDialogscorched.obj ..\src\dialogs\KillDialog.cpp

..\src\scorched\release\srcdialogsMainMenuDialogscorched.obj : ..\src\dialogs\MainMenuDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsMainMenuDialogscorched.obj ..\src\dialogs\MainMenuDialog.cpp

..\src\scorched\release\srcdialogsOptionsDialogscorched.obj : ..\src\dialogs\OptionsDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsOptionsDialogscorched.obj ..\src\dialogs\OptionsDialog.cpp

..\src\scorched\release\srcdialogsPlanViewDialogscorched.obj : ..\src\dialogs\PlanViewDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsPlanViewDialogscorched.obj ..\src\dialogs\PlanViewDialog.cpp

..\src\scorched\release\srcdialogsPlayerDialogscorched.obj : ..\src\dialogs\PlayerDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsPlayerDialogscorched.obj ..\src\dialogs\PlayerDialog.cpp

..\src\scorched\release\srcdialogsPlayersDialogscorched.obj : ..\src\dialogs\PlayersDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsPlayersDialogscorched.obj ..\src\dialogs\PlayersDialog.cpp

..\src\scorched\release\srcdialogsProgressDialogscorched.obj : ..\src\dialogs\ProgressDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsProgressDialogscorched.obj ..\src\dialogs\ProgressDialog.cpp

..\src\scorched\release\srcdialogsQuitDialogscorched.obj : ..\src\dialogs\QuitDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsQuitDialogscorched.obj ..\src\dialogs\QuitDialog.cpp

..\src\scorched\release\srcdialogsScoreDialogscorched.obj : ..\src\dialogs\ScoreDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsScoreDialogscorched.obj ..\src\dialogs\ScoreDialog.cpp

..\src\scorched\release\srcdialogsTalkDialogscorched.obj : ..\src\dialogs\TalkDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsTalkDialogscorched.obj ..\src\dialogs\TalkDialog.cpp

..\src\scorched\release\srcdialogsTankDialogscorched.obj : ..\src\dialogs\TankDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsTankDialogscorched.obj ..\src\dialogs\TankDialog.cpp

..\src\scorched\release\srcdialogsWindDialogscorched.obj : ..\src\dialogs\WindDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcdialogsWindDialogscorched.obj ..\src\dialogs\WindDialog.cpp

..\src\scorched\release\srczlibadler32scorched.obj : ..\src\zlib\adler32.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srczlibadler32scorched.obj ..\src\zlib\adler32.c

..\src\scorched\release\srczlibcompressscorched.obj : ..\src\zlib\compress.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srczlibcompressscorched.obj ..\src\zlib\compress.c

..\src\scorched\release\srczlibcrc32scorched.obj : ..\src\zlib\crc32.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srczlibcrc32scorched.obj ..\src\zlib\crc32.c

..\src\scorched\release\srczlibdeflatescorched.obj : ..\src\zlib\deflate.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srczlibdeflatescorched.obj ..\src\zlib\deflate.c

..\src\scorched\release\srczlibgzioscorched.obj : ..\src\zlib\gzio.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srczlibgzioscorched.obj ..\src\zlib\gzio.c

..\src\scorched\release\srczlibinfblockscorched.obj : ..\src\zlib\infblock.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srczlibinfblockscorched.obj ..\src\zlib\infblock.c

..\src\scorched\release\srczlibinfcodesscorched.obj : ..\src\zlib\infcodes.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srczlibinfcodesscorched.obj ..\src\zlib\infcodes.c

..\src\scorched\release\srczlibinffastscorched.obj : ..\src\zlib\inffast.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srczlibinffastscorched.obj ..\src\zlib\inffast.c

..\src\scorched\release\srczlibinflatescorched.obj : ..\src\zlib\inflate.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srczlibinflatescorched.obj ..\src\zlib\inflate.c

..\src\scorched\release\srczlibinftreesscorched.obj : ..\src\zlib\inftrees.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srczlibinftreesscorched.obj ..\src\zlib\inftrees.c

..\src\scorched\release\srczlibinfutilscorched.obj : ..\src\zlib\infutil.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srczlibinfutilscorched.obj ..\src\zlib\infutil.c

..\src\scorched\release\srczlibtreesscorched.obj : ..\src\zlib\trees.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srczlibtreesscorched.obj ..\src\zlib\trees.c

..\src\scorched\release\srczlibuncomprscorched.obj : ..\src\zlib\uncompr.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srczlibuncomprscorched.obj ..\src\zlib\uncompr.c

..\src\scorched\release\srczlibzutilscorched.obj : ..\src\zlib\zutil.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srczlibzutilscorched.obj ..\src\zlib\zutil.c

..\src\scorched\release\srcengineActionscorched.obj : ..\src\engine\Action.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcengineActionscorched.obj ..\src\engine\Action.cpp

..\src\scorched\release\srcengineActionBufferscorched.obj : ..\src\engine\ActionBuffer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcengineActionBufferscorched.obj ..\src\engine\ActionBuffer.cpp

..\src\scorched\release\srcengineActionControllerscorched.obj : ..\src\engine\ActionController.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcengineActionControllerscorched.obj ..\src\engine\ActionController.cpp

..\src\scorched\release\srcengineActionMetascorched.obj : ..\src\engine\ActionMeta.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcengineActionMetascorched.obj ..\src\engine\ActionMeta.cpp

..\src\scorched\release\srcengineFrameTimerscorched.obj : ..\src\engine\FrameTimer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcengineFrameTimerscorched.obj ..\src\engine\FrameTimer.cpp

..\src\scorched\release\srcengineGameStatescorched.obj : ..\src\engine\GameState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcengineGameStatescorched.obj ..\src\engine\GameState.cpp

..\src\scorched\release\srcengineGameStateIscorched.obj : ..\src\engine\GameStateI.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcengineGameStateIscorched.obj ..\src\engine\GameStateI.cpp

..\src\scorched\release\srcengineGameStateStimulusIscorched.obj : ..\src\engine\GameStateStimulusI.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcengineGameStateStimulusIscorched.obj ..\src\engine\GameStateStimulusI.cpp

..\src\scorched\release\srcengineMainLoopscorched.obj : ..\src\engine\MainLoop.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcengineMainLoopscorched.obj ..\src\engine\MainLoop.cpp

..\src\scorched\release\srcengineMainLoopIscorched.obj : ..\src\engine\MainLoopI.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcengineMainLoopIscorched.obj ..\src\engine\MainLoopI.cpp

..\src\scorched\release\srcenginePhysicsEnginescorched.obj : ..\src\engine\PhysicsEngine.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcenginePhysicsEnginescorched.obj ..\src\engine\PhysicsEngine.cpp

..\src\scorched\release\srcenginePhysicsParticlescorched.obj : ..\src\engine\PhysicsParticle.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcenginePhysicsParticlescorched.obj ..\src\engine\PhysicsParticle.cpp

..\src\scorched\release\srcenginePhysicsParticleObjectscorched.obj : ..\src\engine\PhysicsParticleObject.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcenginePhysicsParticleObjectscorched.obj ..\src\engine\PhysicsParticleObject.cpp

..\src\scorched\release\srcengineScorchedCollisionHandlerscorched.obj : ..\src\engine\ScorchedCollisionHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcengineScorchedCollisionHandlerscorched.obj ..\src\engine\ScorchedCollisionHandler.cpp

..\src\scorched\release\srcengineScorchedCollisionIdsscorched.obj : ..\src\engine\ScorchedCollisionIds.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcengineScorchedCollisionIdsscorched.obj ..\src\engine\ScorchedCollisionIds.cpp

..\src\scorched\release\srcGLWGLWButtonscorched.obj : ..\src\GLW\GLWButton.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWButtonscorched.obj ..\src\GLW\GLWButton.cpp

..\src\scorched\release\srcGLWGLWCheckBoxscorched.obj : ..\src\GLW\GLWCheckBox.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWCheckBoxscorched.obj ..\src\GLW\GLWCheckBox.cpp

..\src\scorched\release\srcGLWGLWDropDownscorched.obj : ..\src\GLW\GLWDropDown.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWDropDownscorched.obj ..\src\GLW\GLWDropDown.cpp

..\src\scorched\release\srcGLWGLWFileViewscorched.obj : ..\src\GLW\GLWFileView.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWFileViewscorched.obj ..\src\GLW\GLWFileView.cpp

..\src\scorched\release\srcGLWGLWFlagscorched.obj : ..\src\GLW\GLWFlag.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWFlagscorched.obj ..\src\GLW\GLWFlag.cpp

..\src\scorched\release\srcGLWGLWFontscorched.obj : ..\src\GLW\GLWFont.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWFontscorched.obj ..\src\GLW\GLWFont.cpp

..\src\scorched\release\srcGLWGLWLabelscorched.obj : ..\src\GLW\GLWLabel.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWLabelscorched.obj ..\src\GLW\GLWLabel.cpp

..\src\scorched\release\srcGLWGLWListViewscorched.obj : ..\src\GLW\GLWListView.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWListViewscorched.obj ..\src\GLW\GLWListView.cpp

..\src\scorched\release\srcGLWGLWPanelscorched.obj : ..\src\GLW\GLWPanel.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWPanelscorched.obj ..\src\GLW\GLWPanel.cpp

..\src\scorched\release\srcGLWGLWProgressscorched.obj : ..\src\GLW\GLWProgress.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWProgressscorched.obj ..\src\GLW\GLWProgress.cpp

..\src\scorched\release\srcGLWGLWPushButtonscorched.obj : ..\src\GLW\GLWPushButton.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWPushButtonscorched.obj ..\src\GLW\GLWPushButton.cpp

..\src\scorched\release\srcGLWGLWScrollButtonscorched.obj : ..\src\GLW\GLWScrollButton.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWScrollButtonscorched.obj ..\src\GLW\GLWScrollButton.cpp

..\src\scorched\release\srcGLWGLWScrollHscorched.obj : ..\src\GLW\GLWScrollH.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWScrollHscorched.obj ..\src\GLW\GLWScrollH.cpp

..\src\scorched\release\srcGLWGLWScrollPanelscorched.obj : ..\src\GLW\GLWScrollPanel.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWScrollPanelscorched.obj ..\src\GLW\GLWScrollPanel.cpp

..\src\scorched\release\srcGLWGLWScrollWscorched.obj : ..\src\GLW\GLWScrollW.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWScrollWscorched.obj ..\src\GLW\GLWScrollW.cpp

..\src\scorched\release\srcGLWGLWSpinBoxscorched.obj : ..\src\GLW\GLWSpinBox.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWSpinBoxscorched.obj ..\src\GLW\GLWSpinBox.cpp

..\src\scorched\release\srcGLWGLWTabscorched.obj : ..\src\GLW\GLWTab.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWTabscorched.obj ..\src\GLW\GLWTab.cpp

..\src\scorched\release\srcGLWGLWTextBoxscorched.obj : ..\src\GLW\GLWTextBox.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWTextBoxscorched.obj ..\src\GLW\GLWTextBox.cpp

..\src\scorched\release\srcGLWGLWTextButtonscorched.obj : ..\src\GLW\GLWTextButton.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWTextButtonscorched.obj ..\src\GLW\GLWTextButton.cpp

..\src\scorched\release\srcGLWGLWVisiblePanelscorched.obj : ..\src\GLW\GLWVisiblePanel.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWVisiblePanelscorched.obj ..\src\GLW\GLWVisiblePanel.cpp

..\src\scorched\release\srcGLWGLWVisibleWidgetscorched.obj : ..\src\GLW\GLWVisibleWidget.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWVisibleWidgetscorched.obj ..\src\GLW\GLWVisibleWidget.cpp

..\src\scorched\release\srcGLWGLWWindowscorched.obj : ..\src\GLW\GLWWindow.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWWindowscorched.obj ..\src\GLW\GLWWindow.cpp

..\src\scorched\release\srcGLWGLWidgetscorched.obj : ..\src\GLW\GLWidget.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLWGLWidgetscorched.obj ..\src\GLW\GLWidget.cpp

..\src\scorched\release\srctankgraphGLWTankViewerscorched.obj : ..\src\tankgraph\GLWTankViewer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankgraphGLWTankViewerscorched.obj ..\src\tankgraph\GLWTankViewer.cpp

..\src\scorched\release\srctankgraphTankMenusscorched.obj : ..\src\tankgraph\TankMenus.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankgraphTankMenusscorched.obj ..\src\tankgraph\TankMenus.cpp

..\src\scorched\release\srctankgraphTankMeshscorched.obj : ..\src\tankgraph\TankMesh.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankgraphTankMeshscorched.obj ..\src\tankgraph\TankMesh.cpp

..\src\scorched\release\srctankgraphTankModelscorched.obj : ..\src\tankgraph\TankModel.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankgraphTankModelscorched.obj ..\src\tankgraph\TankModel.cpp

..\src\scorched\release\srctankgraphTankModelRendererscorched.obj : ..\src\tankgraph\TankModelRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankgraphTankModelRendererscorched.obj ..\src\tankgraph\TankModelRenderer.cpp

..\src\scorched\release\srctankgraphTankModelStorescorched.obj : ..\src\tankgraph\TankModelStore.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankgraphTankModelStorescorched.obj ..\src\tankgraph\TankModelStore.cpp

..\src\scorched\release\srctankgraphTankRendererscorched.obj : ..\src\tankgraph\TankRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankgraphTankRendererscorched.obj ..\src\tankgraph\TankRenderer.cpp

..\src\scorched\release\srctankgraphTracerStorescorched.obj : ..\src\tankgraph\TracerStore.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankgraphTracerStorescorched.obj ..\src\tankgraph\TracerStore.cpp

..\src\scorched\release\srcspritesDebrisActionRendererscorched.obj : ..\src\sprites\DebrisActionRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcspritesDebrisActionRendererscorched.obj ..\src\sprites\DebrisActionRenderer.cpp

..\src\scorched\release\srcspritesExplosionRendererscorched.obj : ..\src\sprites\ExplosionRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcspritesExplosionRendererscorched.obj ..\src\sprites\ExplosionRenderer.cpp

..\src\scorched\release\srcspritesExplosionTexturesscorched.obj : ..\src\sprites\ExplosionTextures.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcspritesExplosionTexturesscorched.obj ..\src\sprites\ExplosionTextures.cpp

..\src\scorched\release\srcspritesFlareActionRendererscorched.obj : ..\src\sprites\FlareActionRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcspritesFlareActionRendererscorched.obj ..\src\sprites\FlareActionRenderer.cpp

..\src\scorched\release\srcspritesSmokeActionRendererscorched.obj : ..\src\sprites\SmokeActionRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcspritesSmokeActionRendererscorched.obj ..\src\sprites\SmokeActionRenderer.cpp

..\src\scorched\release\srcspritesTalkRendererscorched.obj : ..\src\sprites\TalkRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcspritesTalkRendererscorched.obj ..\src\sprites\TalkRenderer.cpp

..\src\scorched\release\srcspritesTankDeadRendererscorched.obj : ..\src\sprites\TankDeadRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcspritesTankDeadRendererscorched.obj ..\src\sprites\TankDeadRenderer.cpp

..\src\scorched\release\srcspritesTextActionRendererscorched.obj : ..\src\sprites\TextActionRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcspritesTextActionRendererscorched.obj ..\src\sprites\TextActionRenderer.cpp

..\src\scorched\release\srcspritesTracerActionRendererscorched.obj : ..\src\sprites\TracerActionRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcspritesTracerActionRendererscorched.obj ..\src\sprites\TracerActionRenderer.cpp

..\src\scorched\release\srcspritesWallActionRendererscorched.obj : ..\src\sprites\WallActionRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcspritesWallActionRendererscorched.obj ..\src\sprites\WallActionRenderer.cpp

..\src\scorched\release\srcactionsExplosionscorched.obj : ..\src\actions\Explosion.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsExplosionscorched.obj ..\src\actions\Explosion.cpp

..\src\scorched\release\srcactionsShieldHitscorched.obj : ..\src\actions\ShieldHit.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsShieldHitscorched.obj ..\src\actions\ShieldHit.cpp

..\src\scorched\release\srcactionsShotProjectilescorched.obj : ..\src\actions\ShotProjectile.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsShotProjectilescorched.obj ..\src\actions\ShotProjectile.cpp

..\src\scorched\release\srcactionsShotProjectileClodscorched.obj : ..\src\actions\ShotProjectileClod.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsShotProjectileClodscorched.obj ..\src\actions\ShotProjectileClod.cpp

..\src\scorched\release\srcactionsShotProjectileExplosionscorched.obj : ..\src\actions\ShotProjectileExplosion.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsShotProjectileExplosionscorched.obj ..\src\actions\ShotProjectileExplosion.cpp

..\src\scorched\release\srcactionsShotProjectileFunkyscorched.obj : ..\src\actions\ShotProjectileFunky.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsShotProjectileFunkyscorched.obj ..\src\actions\ShotProjectileFunky.cpp

..\src\scorched\release\srcactionsShotProjectileMirvscorched.obj : ..\src\actions\ShotProjectileMirv.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsShotProjectileMirvscorched.obj ..\src\actions\ShotProjectileMirv.cpp

..\src\scorched\release\srcactionsShotProjectileRiotscorched.obj : ..\src\actions\ShotProjectileRiot.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsShotProjectileRiotscorched.obj ..\src\actions\ShotProjectileRiot.cpp

..\src\scorched\release\srcactionsShotProjectileTracerscorched.obj : ..\src\actions\ShotProjectileTracer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsShotProjectileTracerscorched.obj ..\src\actions\ShotProjectileTracer.cpp

..\src\scorched\release\srcactionsSpriteProjectilescorched.obj : ..\src\actions\SpriteProjectile.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsSpriteProjectilescorched.obj ..\src\actions\SpriteProjectile.cpp

..\src\scorched\release\srcactionsTankDamagescorched.obj : ..\src\actions\TankDamage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsTankDamagescorched.obj ..\src\actions\TankDamage.cpp

..\src\scorched\release\srcactionsTankDeadscorched.obj : ..\src\actions\TankDead.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsTankDeadscorched.obj ..\src\actions\TankDead.cpp

..\src\scorched\release\srcactionsTankFallingscorched.obj : ..\src\actions\TankFalling.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsTankFallingscorched.obj ..\src\actions\TankFalling.cpp

..\src\scorched\release\srcactionsTankFiredscorched.obj : ..\src\actions\TankFired.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsTankFiredscorched.obj ..\src\actions\TankFired.cpp

..\src\scorched\release\srcactionsTankMovescorched.obj : ..\src\actions\TankMove.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsTankMovescorched.obj ..\src\actions\TankMove.cpp

..\src\scorched\release\srcactionsTankResignscorched.obj : ..\src\actions\TankResign.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsTankResignscorched.obj ..\src\actions\TankResign.cpp

..\src\scorched\release\srcactionsTankSayscorched.obj : ..\src\actions\TankSay.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsTankSayscorched.obj ..\src\actions\TankSay.cpp

..\src\scorched\release\srcactionsTankScoredscorched.obj : ..\src\actions\TankScored.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsTankScoredscorched.obj ..\src\actions\TankScored.cpp

..\src\scorched\release\srcactionsWallHitscorched.obj : ..\src\actions\WallHit.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcactionsWallHitscorched.obj ..\src\actions\WallHit.cpp

..\src\scorched\release\src3dsparseASEArrayFactscorched.obj : ..\src\3dsparse\ASEArrayFact.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\src3dsparseASEArrayFactscorched.obj ..\src\3dsparse\ASEArrayFact.cpp

..\src\scorched\release\src3dsparseASEFilescorched.obj : ..\src\3dsparse\ASEFile.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\src3dsparseASEFilescorched.obj ..\src\3dsparse\ASEFile.cpp

..\src\scorched\release\src3dsparseASEModelscorched.obj : ..\src\3dsparse\ASEModel.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\src3dsparseASEModelscorched.obj ..\src\3dsparse\ASEModel.cpp

..\src\scorched\release\src3dsparseASEStorescorched.obj : ..\src\3dsparse\ASEStore.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\src3dsparseASEStorescorched.obj ..\src\3dsparse\ASEStore.cpp

..\src\scorched\release\src3dsparseMeshLODscorched.obj : ..\src\3dsparse\MeshLOD.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\src3dsparseMeshLODscorched.obj ..\src\3dsparse\MeshLOD.cpp

..\src\scorched\release\src3dsparseMeshLODTriscorched.obj : ..\src\3dsparse\MeshLODTri.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\src3dsparseMeshLODTriscorched.obj ..\src\3dsparse\MeshLODTri.cpp

..\src\scorched\release\src3dsparseMeshLODVectorscorched.obj : ..\src\3dsparse\MeshLODVector.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\src3dsparseMeshLODVectorscorched.obj ..\src\3dsparse\MeshLODVector.cpp

..\src\scorched\release\src3dsparseaseFile.lexscorched.obj : ..\src\3dsparse\aseFile.lex.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\src3dsparseaseFile.lexscorched.obj ..\src\3dsparse\aseFile.lex.cpp

..\src\scorched\release\src3dsparseaseFile.tabscorched.obj : ..\src\3dsparse\aseFile.tab.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\src3dsparseaseFile.tabscorched.obj ..\src\3dsparse\aseFile.tab.cpp

..\src\scorched\release\srctankaiTankAIComputerscorched.obj : ..\src\tankai\TankAIComputer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankaiTankAIComputerscorched.obj ..\src\tankai\TankAIComputer.cpp

..\src\scorched\release\srctankaiTankAIComputerBuyerscorched.obj : ..\src\tankai\TankAIComputerBuyer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankaiTankAIComputerBuyerscorched.obj ..\src\tankai\TankAIComputerBuyer.cpp

..\src\scorched\release\srctankaiTankAIComputerChooserscorched.obj : ..\src\tankai\TankAIComputerChooser.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankaiTankAIComputerChooserscorched.obj ..\src\tankai\TankAIComputerChooser.cpp

..\src\scorched\release\srctankaiTankAIComputerCtrlscorched.obj : ..\src\tankai\TankAIComputerCtrl.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankaiTankAIComputerCtrlscorched.obj ..\src\tankai\TankAIComputerCtrl.cpp

..\src\scorched\release\srctankaiTankAIComputerDHTestscorched.obj : ..\src\tankai\TankAIComputerDHTest.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankaiTankAIComputerDHTestscorched.obj ..\src\tankai\TankAIComputerDHTest.cpp

..\src\scorched\release\srctankaiTankAIComputerMoronscorched.obj : ..\src\tankai\TankAIComputerMoron.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankaiTankAIComputerMoronscorched.obj ..\src\tankai\TankAIComputerMoron.cpp

..\src\scorched\release\srctankaiTankAIComputerPSharkscorched.obj : ..\src\tankai\TankAIComputerPShark.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankaiTankAIComputerPSharkscorched.obj ..\src\tankai\TankAIComputerPShark.cpp

..\src\scorched\release\srctankaiTankAIComputerRandomscorched.obj : ..\src\tankai\TankAIComputerRandom.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankaiTankAIComputerRandomscorched.obj ..\src\tankai\TankAIComputerRandom.cpp

..\src\scorched\release\srctankaiTankAIComputerShooterscorched.obj : ..\src\tankai\TankAIComputerShooter.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankaiTankAIComputerShooterscorched.obj ..\src\tankai\TankAIComputerShooter.cpp

..\src\scorched\release\srctankaiTankAIComputerTosserscorched.obj : ..\src\tankai\TankAIComputerTosser.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankaiTankAIComputerTosserscorched.obj ..\src\tankai\TankAIComputerTosser.cpp

..\src\scorched\release\srctankaiTankAIHumanscorched.obj : ..\src\tankai\TankAIHuman.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankaiTankAIHumanscorched.obj ..\src\tankai\TankAIHuman.cpp

..\src\scorched\release\srctankaiTankAIHumanCtrlscorched.obj : ..\src\tankai\TankAIHumanCtrl.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankaiTankAIHumanCtrlscorched.obj ..\src\tankai\TankAIHumanCtrl.cpp

..\src\scorched\release\srctankaiTankAIStorescorched.obj : ..\src\tankai\TankAIStore.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankaiTankAIStorescorched.obj ..\src\tankai\TankAIStore.cpp

..\src\scorched\release\srctankaiTankAIStringsscorched.obj : ..\src\tankai\TankAIStrings.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srctankaiTankAIStringsscorched.obj ..\src\tankai\TankAIStrings.cpp

..\src\scorched\release\srccomsComsActionsMessagescorched.obj : ..\src\coms\ComsActionsMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsActionsMessagescorched.obj ..\src\coms\ComsActionsMessage.cpp

..\src\scorched\release\srccomsComsAddPlayerMessagescorched.obj : ..\src\coms\ComsAddPlayerMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsAddPlayerMessagescorched.obj ..\src\coms\ComsAddPlayerMessage.cpp

..\src\scorched\release\srccomsComsBuyAccessoryMessagescorched.obj : ..\src\coms\ComsBuyAccessoryMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsBuyAccessoryMessagescorched.obj ..\src\coms\ComsBuyAccessoryMessage.cpp

..\src\scorched\release\srccomsComsConnectAcceptMessagescorched.obj : ..\src\coms\ComsConnectAcceptMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsConnectAcceptMessagescorched.obj ..\src\coms\ComsConnectAcceptMessage.cpp

..\src\scorched\release\srccomsComsConnectMessagescorched.obj : ..\src\coms\ComsConnectMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsConnectMessagescorched.obj ..\src\coms\ComsConnectMessage.cpp

..\src\scorched\release\srccomsComsDefenseMessagescorched.obj : ..\src\coms\ComsDefenseMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsDefenseMessagescorched.obj ..\src\coms\ComsDefenseMessage.cpp

..\src\scorched\release\srccomsComsGameStoppedMessagescorched.obj : ..\src\coms\ComsGameStoppedMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsGameStoppedMessagescorched.obj ..\src\coms\ComsGameStoppedMessage.cpp

..\src\scorched\release\srccomsComsLevelMessagescorched.obj : ..\src\coms\ComsLevelMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsLevelMessagescorched.obj ..\src\coms\ComsLevelMessage.cpp

..\src\scorched\release\srccomsComsMessagescorched.obj : ..\src\coms\ComsMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsMessagescorched.obj ..\src\coms\ComsMessage.cpp

..\src\scorched\release\srccomsComsMessageHandlerscorched.obj : ..\src\coms\ComsMessageHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsMessageHandlerscorched.obj ..\src\coms\ComsMessageHandler.cpp

..\src\scorched\release\srccomsComsMessageSenderscorched.obj : ..\src\coms\ComsMessageSender.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsMessageSenderscorched.obj ..\src\coms\ComsMessageSender.cpp

..\src\scorched\release\srccomsComsNewGameMessagescorched.obj : ..\src\coms\ComsNewGameMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsNewGameMessagescorched.obj ..\src\coms\ComsNewGameMessage.cpp

..\src\scorched\release\srccomsComsPlayedMoveMessagescorched.obj : ..\src\coms\ComsPlayedMoveMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsPlayedMoveMessagescorched.obj ..\src\coms\ComsPlayedMoveMessage.cpp

..\src\scorched\release\srccomsComsPlayerReadyMessagescorched.obj : ..\src\coms\ComsPlayerReadyMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsPlayerReadyMessagescorched.obj ..\src\coms\ComsPlayerReadyMessage.cpp

..\src\scorched\release\srccomsComsPlayerStateMessagescorched.obj : ..\src\coms\ComsPlayerStateMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsPlayerStateMessagescorched.obj ..\src\coms\ComsPlayerStateMessage.cpp

..\src\scorched\release\srccomsComsRmPlayerMessagescorched.obj : ..\src\coms\ComsRmPlayerMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsRmPlayerMessagescorched.obj ..\src\coms\ComsRmPlayerMessage.cpp

..\src\scorched\release\srccomsComsScoreMessagescorched.obj : ..\src\coms\ComsScoreMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsScoreMessagescorched.obj ..\src\coms\ComsScoreMessage.cpp

..\src\scorched\release\srccomsComsStartGameMessagescorched.obj : ..\src\coms\ComsStartGameMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsStartGameMessagescorched.obj ..\src\coms\ComsStartGameMessage.cpp

..\src\scorched\release\srccomsComsTextMessagescorched.obj : ..\src\coms\ComsTextMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsComsTextMessagescorched.obj ..\src\coms\ComsTextMessage.cpp

..\src\scorched\release\srccomsNetBufferscorched.obj : ..\src\coms\NetBuffer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsNetBufferscorched.obj ..\src\coms\NetBuffer.cpp

..\src\scorched\release\srccomsNetBufferUtilscorched.obj : ..\src\coms\NetBufferUtil.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsNetBufferUtilscorched.obj ..\src\coms\NetBufferUtil.cpp

..\src\scorched\release\srccomsNetClientscorched.obj : ..\src\coms\NetClient.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsNetClientscorched.obj ..\src\coms\NetClient.cpp

..\src\scorched\release\srccomsNetMessagescorched.obj : ..\src\coms\NetMessage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsNetMessagescorched.obj ..\src\coms\NetMessage.cpp

..\src\scorched\release\srccomsNetMessageHandlerscorched.obj : ..\src\coms\NetMessageHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsNetMessageHandlerscorched.obj ..\src\coms\NetMessageHandler.cpp

..\src\scorched\release\srccomsNetMessagePoolscorched.obj : ..\src\coms\NetMessagePool.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsNetMessagePoolscorched.obj ..\src\coms\NetMessagePool.cpp

..\src\scorched\release\srccomsNetServerscorched.obj : ..\src\coms\NetServer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccomsNetServerscorched.obj ..\src\coms\NetServer.cpp

..\src\scorched\release\srcweaponsAccessoryscorched.obj : ..\src\weapons\Accessory.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcweaponsAccessoryscorched.obj ..\src\weapons\Accessory.cpp

..\src\scorched\release\srcweaponsAccessoryStorescorched.obj : ..\src\weapons\AccessoryStore.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcweaponsAccessoryStorescorched.obj ..\src\weapons\AccessoryStore.cpp

..\src\scorched\release\srcweaponsAutoDefensescorched.obj : ..\src\weapons\AutoDefense.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcweaponsAutoDefensescorched.obj ..\src\weapons\AutoDefense.cpp

..\src\scorched\release\srcweaponsBatteryscorched.obj : ..\src\weapons\Battery.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcweaponsBatteryscorched.obj ..\src\weapons\Battery.cpp

..\src\scorched\release\srcweaponsParachutescorched.obj : ..\src\weapons\Parachute.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcweaponsParachutescorched.obj ..\src\weapons\Parachute.cpp

..\src\scorched\release\srcweaponsShieldscorched.obj : ..\src\weapons\Shield.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcweaponsShieldscorched.obj ..\src\weapons\Shield.cpp

..\src\scorched\release\srcweaponsShieldReflectivescorched.obj : ..\src\weapons\ShieldReflective.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcweaponsShieldReflectivescorched.obj ..\src\weapons\ShieldReflective.cpp

..\src\scorched\release\srcweaponsShieldReflectiveMagscorched.obj : ..\src\weapons\ShieldReflectiveMag.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcweaponsShieldReflectiveMagscorched.obj ..\src\weapons\ShieldReflectiveMag.cpp

..\src\scorched\release\srcweaponsWeaponscorched.obj : ..\src\weapons\Weapon.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcweaponsWeaponscorched.obj ..\src\weapons\Weapon.cpp

..\src\scorched\release\srcweaponsWeaponClodscorched.obj : ..\src\weapons\WeaponClod.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcweaponsWeaponClodscorched.obj ..\src\weapons\WeaponClod.cpp

..\src\scorched\release\srcweaponsWeaponFunkyscorched.obj : ..\src\weapons\WeaponFunky.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcweaponsWeaponFunkyscorched.obj ..\src\weapons\WeaponFunky.cpp

..\src\scorched\release\srcweaponsWeaponMirvscorched.obj : ..\src\weapons\WeaponMirv.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcweaponsWeaponMirvscorched.obj ..\src\weapons\WeaponMirv.cpp

..\src\scorched\release\srcweaponsWeaponProjectilescorched.obj : ..\src\weapons\WeaponProjectile.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcweaponsWeaponProjectilescorched.obj ..\src\weapons\WeaponProjectile.cpp

..\src\scorched\release\srcweaponsWeaponRiotBombscorched.obj : ..\src\weapons\WeaponRiotBomb.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcweaponsWeaponRiotBombscorched.obj ..\src\weapons\WeaponRiotBomb.cpp

..\src\scorched\release\srcweaponsWeaponTracerscorched.obj : ..\src\weapons\WeaponTracer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcweaponsWeaponTracerscorched.obj ..\src\weapons\WeaponTracer.cpp

..\src\scorched\release\srcGLEXTGLBitmapscorched.obj : ..\src\GLEXT\GLBitmap.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLBitmapscorched.obj ..\src\GLEXT\GLBitmap.cpp

..\src\scorched\release\srcGLEXTGLBitmapItteratorscorched.obj : ..\src\GLEXT\GLBitmapItterator.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLBitmapItteratorscorched.obj ..\src\GLEXT\GLBitmapItterator.cpp

..\src\scorched\release\srcGLEXTGLBitmapModifierscorched.obj : ..\src\GLEXT\GLBitmapModifier.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLBitmapModifierscorched.obj ..\src\GLEXT\GLBitmapModifier.cpp

..\src\scorched\release\srcGLEXTGLCamerascorched.obj : ..\src\GLEXT\GLCamera.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLCamerascorched.obj ..\src\GLEXT\GLCamera.cpp

..\src\scorched\release\srcGLEXTGLCameraFrustumscorched.obj : ..\src\GLEXT\GLCameraFrustum.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLCameraFrustumscorched.obj ..\src\GLEXT\GLCameraFrustum.cpp

..\src\scorched\release\srcGLEXTGLConsolescorched.obj : ..\src\GLEXT\GLConsole.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLConsolescorched.obj ..\src\GLEXT\GLConsole.cpp

..\src\scorched\release\srcGLEXTGLConsoleFileReaderscorched.obj : ..\src\GLEXT\GLConsoleFileReader.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLConsoleFileReaderscorched.obj ..\src\GLEXT\GLConsoleFileReader.cpp

..\src\scorched\release\srcGLEXTGLConsoleLinesscorched.obj : ..\src\GLEXT\GLConsoleLines.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLConsoleLinesscorched.obj ..\src\GLEXT\GLConsoleLines.cpp

..\src\scorched\release\srcGLEXTGLConsoleMethodsscorched.obj : ..\src\GLEXT\GLConsoleMethods.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLConsoleMethodsscorched.obj ..\src\GLEXT\GLConsoleMethods.cpp

..\src\scorched\release\srcGLEXTGLConsoleRulescorched.obj : ..\src\GLEXT\GLConsoleRule.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLConsoleRulescorched.obj ..\src\GLEXT\GLConsoleRule.cpp

..\src\scorched\release\srcGLEXTGLConsoleRuleFnscorched.obj : ..\src\GLEXT\GLConsoleRuleFn.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLConsoleRuleFnscorched.obj ..\src\GLEXT\GLConsoleRuleFn.cpp

..\src\scorched\release\srcGLEXTGLConsoleRuleFnIAdapterscorched.obj : ..\src\GLEXT\GLConsoleRuleFnIAdapter.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLConsoleRuleFnIAdapterscorched.obj ..\src\GLEXT\GLConsoleRuleFnIAdapter.cpp

..\src\scorched\release\srcGLEXTGLConsoleRuleMethodscorched.obj : ..\src\GLEXT\GLConsoleRuleMethod.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLConsoleRuleMethodscorched.obj ..\src\GLEXT\GLConsoleRuleMethod.cpp

..\src\scorched\release\srcGLEXTGLConsoleRulesscorched.obj : ..\src\GLEXT\GLConsoleRules.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLConsoleRulesscorched.obj ..\src\GLEXT\GLConsoleRules.cpp

..\src\scorched\release\srcGLEXTGLFont2dscorched.obj : ..\src\GLEXT\GLFont2d.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLFont2dscorched.obj ..\src\GLEXT\GLFont2d.cpp

..\src\scorched\release\srcGLEXTGLFontBannerscorched.obj : ..\src\GLEXT\GLFontBanner.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLFontBannerscorched.obj ..\src\GLEXT\GLFontBanner.cpp

..\src\scorched\release\srcGLEXTGLImagescorched.obj : ..\src\GLEXT\GLImage.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLImagescorched.obj ..\src\GLEXT\GLImage.cpp

..\src\scorched\release\srcGLEXTGLLenseFlarescorched.obj : ..\src\GLEXT\GLLenseFlare.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLLenseFlarescorched.obj ..\src\GLEXT\GLLenseFlare.cpp

..\src\scorched\release\srcGLEXTGLLuminancescorched.obj : ..\src\GLEXT\GLLuminance.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLLuminancescorched.obj ..\src\GLEXT\GLLuminance.cpp

..\src\scorched\release\srcGLEXTGLMenuscorched.obj : ..\src\GLEXT\GLMenu.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLMenuscorched.obj ..\src\GLEXT\GLMenu.cpp

..\src\scorched\release\srcGLEXTGLMenuEntryscorched.obj : ..\src\GLEXT\GLMenuEntry.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLMenuEntryscorched.obj ..\src\GLEXT\GLMenuEntry.cpp

..\src\scorched\release\srcGLEXTGLMenuIscorched.obj : ..\src\GLEXT\GLMenuI.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLMenuIscorched.obj ..\src\GLEXT\GLMenuI.cpp

..\src\scorched\release\srcGLEXTGLStatescorched.obj : ..\src\GLEXT\GLState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLStatescorched.obj ..\src\GLEXT\GLState.cpp

..\src\scorched\release\srcGLEXTGLStateExtensionscorched.obj : ..\src\GLEXT\GLStateExtension.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLStateExtensionscorched.obj ..\src\GLEXT\GLStateExtension.cpp

..\src\scorched\release\srcGLEXTGLTexturescorched.obj : ..\src\GLEXT\GLTexture.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLTexturescorched.obj ..\src\GLEXT\GLTexture.cpp

..\src\scorched\release\srcGLEXTGLTextureCubeMapscorched.obj : ..\src\GLEXT\GLTextureCubeMap.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLTextureCubeMapscorched.obj ..\src\GLEXT\GLTextureCubeMap.cpp

..\src\scorched\release\srcGLEXTGLTextureSetscorched.obj : ..\src\GLEXT\GLTextureSet.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLTextureSetscorched.obj ..\src\GLEXT\GLTextureSet.cpp

..\src\scorched\release\srcGLEXTGLVarscorched.obj : ..\src\GLEXT\GLVar.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLVarscorched.obj ..\src\GLEXT\GLVar.cpp

..\src\scorched\release\srcGLEXTGLVertexArrayscorched.obj : ..\src\GLEXT\GLVertexArray.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLVertexArrayscorched.obj ..\src\GLEXT\GLVertexArray.cpp

..\src\scorched\release\srcGLEXTGLVertexTexArrayscorched.obj : ..\src\GLEXT\GLVertexTexArray.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcGLEXTGLVertexTexArrayscorched.obj ..\src\GLEXT\GLVertexTexArray.cpp

..\src\scorched\release\srccommonARGParserscorched.obj : ..\src\common\ARGParser.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonARGParserscorched.obj ..\src\common\ARGParser.cpp

..\src\scorched\release\srccommonDefinesscorched.obj : ..\src\common\Defines.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonDefinesscorched.obj ..\src\common\Defines.cpp

..\src\scorched\release\srccommonDisplayscorched.obj : ..\src\common\Display.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonDisplayscorched.obj ..\src\common\Display.cpp

..\src\scorched\release\srccommonFacescorched.obj : ..\src\common\Face.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonFacescorched.obj ..\src\common\Face.cpp

..\src\scorched\release\srccommonFileLinesscorched.obj : ..\src\common\FileLines.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonFileLinesscorched.obj ..\src\common\FileLines.cpp

..\src\scorched\release\srccommonFileListscorched.obj : ..\src\common\FileList.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonFileListscorched.obj ..\src\common\FileList.cpp

..\src\scorched\release\srccommonGammascorched.obj : ..\src\common\Gamma.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonGammascorched.obj ..\src\common\Gamma.cpp

..\src\scorched\release\srccommonKeyboardscorched.obj : ..\src\common\Keyboard.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonKeyboardscorched.obj ..\src\common\Keyboard.cpp

..\src\scorched\release\srccommonLinescorched.obj : ..\src\common\Line.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonLinescorched.obj ..\src\common\Line.cpp

..\src\scorched\release\srccommonMousescorched.obj : ..\src\common\Mouse.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonMousescorched.obj ..\src\common\Mouse.cpp

..\src\scorched\release\srccommonOptionEntryscorched.obj : ..\src\common\OptionEntry.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonOptionEntryscorched.obj ..\src\common\OptionEntry.cpp

..\src\scorched\release\srccommonOptionsGamescorched.obj : ..\src\common\OptionsGame.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonOptionsGamescorched.obj ..\src\common\OptionsGame.cpp

..\src\scorched\release\srccommonOptionsTransientscorched.obj : ..\src\common\OptionsTransient.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonOptionsTransientscorched.obj ..\src\common\OptionsTransient.cpp

..\src\scorched\release\srccommonProgressCounterscorched.obj : ..\src\common\ProgressCounter.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonProgressCounterscorched.obj ..\src\common\ProgressCounter.cpp

..\src\scorched\release\srccommonRandomGeneratorscorched.obj : ..\src\common\RandomGenerator.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonRandomGeneratorscorched.obj ..\src\common\RandomGenerator.cpp

..\src\scorched\release\srccommonRegistryscorched.obj : ..\src\common\Registry.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonRegistryscorched.obj ..\src\common\Registry.cpp

..\src\scorched\release\srccommonSoundscorched.obj : ..\src\common\Sound.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonSoundscorched.obj ..\src\common\Sound.cpp

..\src\scorched\release\srccommonSoundBufferscorched.obj : ..\src\common\SoundBuffer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonSoundBufferscorched.obj ..\src\common\SoundBuffer.cpp

..\src\scorched\release\srccommonSoundStorescorched.obj : ..\src\common\SoundStore.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonSoundStorescorched.obj ..\src\common\SoundStore.cpp

..\src\scorched\release\srccommonTimerscorched.obj : ..\src\common\Timer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonTimerscorched.obj ..\src\common\Timer.cpp

..\src\scorched\release\srccommonTrianglescorched.obj : ..\src\common\Triangle.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonTrianglescorched.obj ..\src\common\Triangle.cpp

..\src\scorched\release\srccommonVectorscorched.obj : ..\src\common\Vector.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonVectorscorched.obj ..\src\common\Vector.cpp

..\src\scorched\release\srccommonVectorLibscorched.obj : ..\src\common\VectorLib.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonVectorLibscorched.obj ..\src\common\VectorLib.cpp

..\src\scorched\release\srccommonWindowManagerscorched.obj : ..\src\common\WindowManager.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srccommonWindowManagerscorched.obj ..\src\common\WindowManager.cpp

..\src\scorched\release\srclandscapeCloudSimscorched.obj : ..\src\landscape\CloudSim.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeCloudSimscorched.obj ..\src\landscape\CloudSim.cpp

..\src\scorched\release\srclandscapeDeformLandscapescorched.obj : ..\src\landscape\DeformLandscape.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeDeformLandscapescorched.obj ..\src\landscape\DeformLandscape.cpp

..\src\scorched\release\srclandscapeDeformTexturesscorched.obj : ..\src\landscape\DeformTextures.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeDeformTexturesscorched.obj ..\src\landscape\DeformTextures.cpp

..\src\scorched\release\srclandscapeGlobalHMapscorched.obj : ..\src\landscape\GlobalHMap.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeGlobalHMapscorched.obj ..\src\landscape\GlobalHMap.cpp

..\src\scorched\release\srclandscapeHeightMapscorched.obj : ..\src\landscape\HeightMap.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeHeightMapscorched.obj ..\src\landscape\HeightMap.cpp

..\src\scorched\release\srclandscapeHeightMapCollisionscorched.obj : ..\src\landscape\HeightMapCollision.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeHeightMapCollisionscorched.obj ..\src\landscape\HeightMapCollision.cpp

..\src\scorched\release\srclandscapeHeightMapLoaderscorched.obj : ..\src\landscape\HeightMapLoader.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeHeightMapLoaderscorched.obj ..\src\landscape\HeightMapLoader.cpp

..\src\scorched\release\srclandscapeHeightMapModifierscorched.obj : ..\src\landscape\HeightMapModifier.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeHeightMapModifierscorched.obj ..\src\landscape\HeightMapModifier.cpp

..\src\scorched\release\srclandscapeHeightMapRendererscorched.obj : ..\src\landscape\HeightMapRenderer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeHeightMapRendererscorched.obj ..\src\landscape\HeightMapRenderer.cpp

..\src\scorched\release\srclandscapeHeightMapSurroundscorched.obj : ..\src\landscape\HeightMapSurround.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeHeightMapSurroundscorched.obj ..\src\landscape\HeightMapSurround.cpp

..\src\scorched\release\srclandscapeHemispherescorched.obj : ..\src\landscape\Hemisphere.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeHemispherescorched.obj ..\src\landscape\Hemisphere.cpp

..\src\scorched\release\srclandscapeLanscapescorched.obj : ..\src\landscape\Lanscape.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeLanscapescorched.obj ..\src\landscape\Lanscape.cpp

..\src\scorched\release\srclandscapePatchscorched.obj : ..\src\landscape\Patch.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapePatchscorched.obj ..\src\landscape\Patch.cpp

..\src\scorched\release\srclandscapePatchGridscorched.obj : ..\src\landscape\PatchGrid.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapePatchGridscorched.obj ..\src\landscape\PatchGrid.cpp

..\src\scorched\release\srclandscapePatchSidescorched.obj : ..\src\landscape\PatchSide.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapePatchSidescorched.obj ..\src\landscape\PatchSide.cpp

..\src\scorched\release\srclandscapePatchVarscorched.obj : ..\src\landscape\PatchVar.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapePatchVarscorched.obj ..\src\landscape\PatchVar.cpp

..\src\scorched\release\srclandscapeShadowMapscorched.obj : ..\src\landscape\ShadowMap.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeShadowMapscorched.obj ..\src\landscape\ShadowMap.cpp

..\src\scorched\release\srclandscapeSmokescorched.obj : ..\src\landscape\Smoke.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeSmokescorched.obj ..\src\landscape\Smoke.cpp

..\src\scorched\release\srclandscapeSmokeChainscorched.obj : ..\src\landscape\SmokeChain.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeSmokeChainscorched.obj ..\src\landscape\SmokeChain.cpp

..\src\scorched\release\srclandscapeSunscorched.obj : ..\src\landscape\Sun.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeSunscorched.obj ..\src\landscape\Sun.cpp

..\src\scorched\release\srclandscapeSurroundscorched.obj : ..\src\landscape\Surround.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeSurroundscorched.obj ..\src\landscape\Surround.cpp

..\src\scorched\release\srclandscapeSurroundDefsscorched.obj : ..\src\landscape\SurroundDefs.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeSurroundDefsscorched.obj ..\src\landscape\SurroundDefs.cpp

..\src\scorched\release\srclandscapeTriNodescorched.obj : ..\src\landscape\TriNode.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeTriNodescorched.obj ..\src\landscape\TriNode.cpp

..\src\scorched\release\srclandscapeTriNodePoolscorched.obj : ..\src\landscape\TriNodePool.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeTriNodePoolscorched.obj ..\src\landscape\TriNodePool.cpp

..\src\scorched\release\srclandscapeVariancescorched.obj : ..\src\landscape\Variance.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeVariancescorched.obj ..\src\landscape\Variance.cpp

..\src\scorched\release\srclandscapeWallscorched.obj : ..\src\landscape\Wall.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeWallscorched.obj ..\src\landscape\Wall.cpp

..\src\scorched\release\srclandscapeWaterMapscorched.obj : ..\src\landscape\WaterMap.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeWaterMapscorched.obj ..\src\landscape\WaterMap.cpp

..\src\scorched\release\srclandscapeWaterMapModifierscorched.obj : ..\src\landscape\WaterMapModifier.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeWaterMapModifierscorched.obj ..\src\landscape\WaterMapModifier.cpp

..\src\scorched\release\srclandscapeWaterMapPointsscorched.obj : ..\src\landscape\WaterMapPoints.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeWaterMapPointsscorched.obj ..\src\landscape\WaterMapPoints.cpp

..\src\scorched\release\srclandscapeWaterMapSurroundscorched.obj : ..\src\landscape\WaterMapSurround.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srclandscapeWaterMapSurroundscorched.obj ..\src\landscape\WaterMapSurround.cpp

..\src\scorched\release\srcXMLXMLFilescorched.obj : ..\src\XML\XMLFile.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcXMLXMLFilescorched.obj ..\src\XML\XMLFile.cpp

..\src\scorched\release\srcXMLXMLParserscorched.obj : ..\src\XML\XMLParser.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcXMLXMLParserscorched.obj ..\src\XML\XMLParser.cpp

..\src\scorched\release\srcXMLxmlparsescorched.obj : ..\src\XML\xmlparse.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcXMLxmlparsescorched.obj ..\src\XML\xmlparse.c

..\src\scorched\release\srcXMLxmlrolescorched.obj : ..\src\XML\xmlrole.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcXMLxmlrolescorched.obj ..\src\XML\xmlrole.c

..\src\scorched\release\srcXMLxmltokscorched.obj : ..\src\XML\xmltok.c
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcXMLxmltokscorched.obj ..\src\XML\xmltok.c

..\src\scorched\release\srcclientClientActionsHandlerscorched.obj : ..\src\client\ClientActionsHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientActionsHandlerscorched.obj ..\src\client\ClientActionsHandler.cpp

..\src\scorched\release\srcclientClientAddPlayerHandlerscorched.obj : ..\src\client\ClientAddPlayerHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientAddPlayerHandlerscorched.obj ..\src\client\ClientAddPlayerHandler.cpp

..\src\scorched\release\srcclientClientBuyAccessoryHandlerscorched.obj : ..\src\client\ClientBuyAccessoryHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientBuyAccessoryHandlerscorched.obj ..\src\client\ClientBuyAccessoryHandler.cpp

..\src\scorched\release\srcclientClientConnectionAcceptHandlerscorched.obj : ..\src\client\ClientConnectionAcceptHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientConnectionAcceptHandlerscorched.obj ..\src\client\ClientConnectionAcceptHandler.cpp

..\src\scorched\release\srcclientClientDefenseHandlerscorched.obj : ..\src\client\ClientDefenseHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientDefenseHandlerscorched.obj ..\src\client\ClientDefenseHandler.cpp

..\src\scorched\release\srcclientClientDialogscorched.obj : ..\src\client\ClientDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientDialogscorched.obj ..\src\client\ClientDialog.cpp

..\src\scorched\release\srcclientClientGameStoppedHandlerscorched.obj : ..\src\client\ClientGameStoppedHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientGameStoppedHandlerscorched.obj ..\src\client\ClientGameStoppedHandler.cpp

..\src\scorched\release\srcclientClientMainscorched.obj : ..\src\client\ClientMain.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientMainscorched.obj ..\src\client\ClientMain.cpp

..\src\scorched\release\srcclientClientMessageHandlerscorched.obj : ..\src\client\ClientMessageHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientMessageHandlerscorched.obj ..\src\client\ClientMessageHandler.cpp

..\src\scorched\release\srcclientClientNewGameHandlerscorched.obj : ..\src\client\ClientNewGameHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientNewGameHandlerscorched.obj ..\src\client\ClientNewGameHandler.cpp

..\src\scorched\release\srcclientClientNextRoundStatescorched.obj : ..\src\client\ClientNextRoundState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientNextRoundStatescorched.obj ..\src\client\ClientNextRoundState.cpp

..\src\scorched\release\srcclientClientRmPlayerHandlerscorched.obj : ..\src\client\ClientRmPlayerHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientRmPlayerHandlerscorched.obj ..\src\client\ClientRmPlayerHandler.cpp

..\src\scorched\release\srcclientClientScoreHandlerscorched.obj : ..\src\client\ClientScoreHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientScoreHandlerscorched.obj ..\src\client\ClientScoreHandler.cpp

..\src\scorched\release\srcclientClientShotStatescorched.obj : ..\src\client\ClientShotState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientShotStatescorched.obj ..\src\client\ClientShotState.cpp

..\src\scorched\release\srcclientClientStartGameHandlerscorched.obj : ..\src\client\ClientStartGameHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientStartGameHandlerscorched.obj ..\src\client\ClientStartGameHandler.cpp

..\src\scorched\release\srcclientClientStatescorched.obj : ..\src\client\ClientState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientStatescorched.obj ..\src\client\ClientState.cpp

..\src\scorched\release\srcclientClientTextHandlerscorched.obj : ..\src\client\ClientTextHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientClientTextHandlerscorched.obj ..\src\client\ClientTextHandler.cpp

..\src\scorched\release\srcclientGLSetupscorched.obj : ..\src\client\GLSetup.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientGLSetupscorched.obj ..\src\client\GLSetup.cpp

..\src\scorched\release\srcclientMain2DCamerascorched.obj : ..\src\client\Main2DCamera.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientMain2DCamerascorched.obj ..\src\client\Main2DCamera.cpp

..\src\scorched\release\srcclientMainBannerscorched.obj : ..\src\client\MainBanner.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientMainBannerscorched.obj ..\src\client\MainBanner.cpp

..\src\scorched\release\srcclientMainCamerascorched.obj : ..\src\client\MainCamera.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientMainCamerascorched.obj ..\src\client\MainCamera.cpp

..\src\scorched\release\srcclientOptionsClientscorched.obj : ..\src\client\OptionsClient.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientOptionsClientscorched.obj ..\src\client\OptionsClient.cpp

..\src\scorched\release\srcclientShotTimerscorched.obj : ..\src\client\ShotTimer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientShotTimerscorched.obj ..\src\client\ShotTimer.cpp

..\src\scorched\release\srcclientSpeedChangescorched.obj : ..\src\client\SpeedChange.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientSpeedChangescorched.obj ..\src\client\SpeedChange.cpp

..\src\scorched\release\srcclientWindowSetupscorched.obj : ..\src\client\WindowSetup.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcclientWindowSetupscorched.obj ..\src\client\WindowSetup.cpp

..\src\scorched\release\srcserverOptionsServerscorched.obj : ..\src\server\OptionsServer.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverOptionsServerscorched.obj ..\src\server\OptionsServer.cpp

..\src\scorched\release\srcserverScorchedASEscorched.obj : ..\src\server\ScorchedASE.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverScorchedASEscorched.obj ..\src\server\ScorchedASE.cpp

..\src\scorched\release\srcserverServerBuyAccessoryHandlerscorched.obj : ..\src\server\ServerBuyAccessoryHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerBuyAccessoryHandlerscorched.obj ..\src\server\ServerBuyAccessoryHandler.cpp

..\src\scorched\release\srcserverServerConnectHandlerscorched.obj : ..\src\server\ServerConnectHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerConnectHandlerscorched.obj ..\src\server\ServerConnectHandler.cpp

..\src\scorched\release\srcserverServerDefenseHandlerscorched.obj : ..\src\server\ServerDefenseHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerDefenseHandlerscorched.obj ..\src\server\ServerDefenseHandler.cpp

..\src\scorched\release\srcserverServerDialogscorched.obj : ..\src\server\ServerDialog.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerDialogscorched.obj ..\src\server\ServerDialog.cpp

..\src\scorched\release\srcserverServerMainscorched.obj : ..\src\server\ServerMain.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerMainscorched.obj ..\src\server\ServerMain.cpp

..\src\scorched\release\srcserverServerMessageHandlerscorched.obj : ..\src\server\ServerMessageHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerMessageHandlerscorched.obj ..\src\server\ServerMessageHandler.cpp

..\src\scorched\release\srcserverServerNewGameStatescorched.obj : ..\src\server\ServerNewGameState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerNewGameStatescorched.obj ..\src\server\ServerNewGameState.cpp

..\src\scorched\release\srcserverServerNextRoundStatescorched.obj : ..\src\server\ServerNextRoundState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerNextRoundStatescorched.obj ..\src\server\ServerNextRoundState.cpp

..\src\scorched\release\srcserverServerPlayedMoveHandlerscorched.obj : ..\src\server\ServerPlayedMoveHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerPlayedMoveHandlerscorched.obj ..\src\server\ServerPlayedMoveHandler.cpp

..\src\scorched\release\srcserverServerPlayerReadyHandlerscorched.obj : ..\src\server\ServerPlayerReadyHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerPlayerReadyHandlerscorched.obj ..\src\server\ServerPlayerReadyHandler.cpp

..\src\scorched\release\srcserverServerPlayingStatescorched.obj : ..\src\server\ServerPlayingState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerPlayingStatescorched.obj ..\src\server\ServerPlayingState.cpp

..\src\scorched\release\srcserverServerScoreStatescorched.obj : ..\src\server\ServerScoreState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerScoreStatescorched.obj ..\src\server\ServerScoreState.cpp

..\src\scorched\release\srcserverServerShotHolderscorched.obj : ..\src\server\ServerShotHolder.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerShotHolderscorched.obj ..\src\server\ServerShotHolder.cpp

..\src\scorched\release\srcserverServerShotStatescorched.obj : ..\src\server\ServerShotState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerShotStatescorched.obj ..\src\server\ServerShotState.cpp

..\src\scorched\release\srcserverServerStatescorched.obj : ..\src\server\ServerState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerStatescorched.obj ..\src\server\ServerState.cpp

..\src\scorched\release\srcserverServerTextHandlerscorched.obj : ..\src\server\ServerTextHandler.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerTextHandlerscorched.obj ..\src\server\ServerTextHandler.cpp

..\src\scorched\release\srcserverServerTooFewPlayersStimulusscorched.obj : ..\src\server\ServerTooFewPlayersStimulus.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerTooFewPlayersStimulusscorched.obj ..\src\server\ServerTooFewPlayersStimulus.cpp

..\src\scorched\release\srcserverServerWaitingForPlayersStatescorched.obj : ..\src\server\ServerWaitingForPlayersState.cpp
	$(CC) $(CCFLAGS) $(DEFINESscorched) -c -o..\src\scorched\release\srcserverServerWaitingForPlayersStatescorched.obj ..\src\server\ServerWaitingForPlayersState.cpp

# Other Deps
..\src\scorched\release\srcactionsSpriteProjectilescorched.obj: ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\engine/PhysicsParticleObject.h ..\src\actions\SpriteProjectile.cpp ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\engine/ScorchedCollisionIds.h ..\src\actions/SpriteProjectile.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\ode/config.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcodespacescorched.obj: ..\src\ode/common.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/config.h ..\src\ode\space.cpp ..\src\ode/error.h
..\src\scorched\release\srctankTankScorescorched.obj: ..\src\common/OptionsGame.h ..\src\common/ARGParser.h ..\src\common/FileLines.h ..\src\tank\TankScore.cpp ..\src\common/OptionEntry.h ..\src\tank/TankScore.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srctankaiTankAIComputerCtrlscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\tankai\TankAIComputerCtrl.cpp ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\coms/ComsPlayedMoveMessage.h ..\src\tankai/TankAIComputerCtrl.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\server/ServerState.h ..\src\server/ServerShotHolder.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srclandscapeHeightMapLoaderscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\landscape/HeightMap.h ..\src\landscape/HeightMapModifier.h ..\src\common/Vector.h ..\src\landscape/HeightMapLoader.h ..\src\landscape\HeightMapLoader.cpp ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\common/RandomGenerator.h ..\src\GLEXT/GLBitmap.h
..\src\scorched\release\srcengineActionControllerscorched.obj: ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\engine/ActionBuffer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\engine\ActionController.cpp ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcclientClientDefenseHandlerscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\coms/ComsMessageHandler.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\client\ClientDefenseHandler.cpp ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\coms/ComsDefenseMessage.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\client/ClientDefenseHandler.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srccomsComsDefenseMessagescorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms\ComsDefenseMessage.cpp ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\coms/ComsDefenseMessage.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srcactionsTankFiredscorched.obj: ..\src\common/SoundStore.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\weapons/Weapon.h ..\src\engine/ActionBuffer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\GLEXT/GLTexture.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\GLEXT/GLVertexArray.h ..\src\common/Sound.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\common/SoundBuffer.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\actions\TankFired.cpp ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\tankgraph/TankMesh.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\tankgraph/TankModelRenderer.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\actions/Explosion.h ..\src\tankgraph/TankModel.h ..\src\weapons/AccessoryStore.h ..\src\engine/GameStateI.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\actions/TankFired.h ..\src\engine/Action.h
..\src\scorched\release\srczlibcompressscorched.obj: ..\src\zlib\compress.c
..\src\scorched\release\srclandscapeLanscapescorched.obj: ..\src\GLEXT/GLBitmapModifier.h ..\src\common/Defines.h ..\src\landscape/Variance.h ..\src\GLEXT/GLImage.h ..\src\common/ARGParser.h ..\src\GLEXT/GLStateExtension.h ..\src\landscape/WaterMapModifier.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\landscape/ShadowMap.h ..\src\landscape/PatchGrid.h ..\src\landscape/HeightMap.h ..\src\landscape/GlobalHMap.h ..\src\landscape/Surround.h ..\src\common/FileLines.h ..\src\common/Vector.h ..\src\landscape/Wall.h ..\src\landscape/WaterMapPoints.h ..\src\landscape/HeightMapRenderer.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\GLW/GLWWindow.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWidget.h ..\src\landscape/Smoke.h ..\src\landscape/Landscape.h ..\src\GLEXT/GLVertexArray.h ..\src\landscape/WaterMap.h ..\src\common/OptionEntry.h ..\src\dialogs/WindDialog.h ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\coms/ComsLevelMessage.h ..\src\landscape/SmokeChain.h ..\src\landscape/Sun.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\landscape/TriNode.h ..\src\landscape/TriNodePool.h ..\src\client/OptionsClient.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\landscape/Patch.h ..\src\landscape/WaterMapSurround.h ..\src\GLW/GLWVisibleWidget.h ..\src\landscape/SurroundDefs.h ..\src\GLEXT/GLBitmap.h ..\src\landscape/HeightMapSurround.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\landscape/DeformLandscape.h ..\src\landscape\Lanscape.cpp ..\src\landscape/PatchSide.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcGLEXTGLConsoleRuleFnscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLConsoleRuleFn.h ..\src\GLEXT/GLConsoleRule.h ..\src\GLEXT\GLConsoleRuleFn.cpp
..\src\scorched\release\srcGLWGLWWindowscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLW/GLWWindow.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLEXT/GLState.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLW\GLWWindow.cpp ..\src\GLEXT/GLBitmap.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcspritesDebrisActionRendererscorched.obj: ..\src\engine/ActionMeta.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\engine/PhysicsParticleObject.h ..\src\3dsparse/ASEStore.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\GLEXT/GLVertexArray.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\sprites/DebrisActionRenderer.h ..\src\GLEXT/GLState.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\ode/config.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\sprites\DebrisActionRenderer.cpp ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\src3dsparseaseFile.lexscorched.obj: ..\src\3dsparse\aseFile.lex.cpp ..\src\common/Vector.h ..\src\3dsparse/aseFile.tab.cpp.h
..\src\scorched\release\srccommonProgressCounterscorched.obj: ..\src\common\ProgressCounter.cpp ..\src\common/ProgressCounter.h
..\src\scorched\release\srccomsComsMessageHandlerscorched.obj: ..\src\common/Defines.h ..\src\coms\ComsMessageHandler.cpp ..\src\coms/NetMessage.h ..\src\coms/NetMessageHandler.h ..\src\coms/ComsMessageHandler.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srcGLEXTGLImagescorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLState.h ..\src\GLEXT\GLImage.cpp
..\src\scorched\release\srccommonDefinesscorched.obj: ..\src\common\Defines.cpp
..\src\scorched\release\srcGLEXTGLVarscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLStateExtension.h ..\src\GLEXT\GLVar.cpp ..\src\GLEXT/GLVar.h ..\src\GLEXT/GLState.h
..\src\scorched\release\srcactionsShotProjectileMirvscorched.obj: ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\engine/PhysicsParticleObject.h ..\src\weapons/Weapon.h ..\src\engine/ActionBuffer.h ..\src\actions/ShotProjectile.h ..\src\ode/error.h ..\src\actions/ShotProjectileExplosion.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\engine/ScorchedCollisionIds.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\weapons/Accessory.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\actions\ShotProjectileMirv.cpp ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\actions/ShotProjectileMirv.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcengineFrameTimerscorched.obj: ..\src\common/Defines.h ..\src\common/ARGParser.h ..\src\common/FileLines.h ..\src\common/Vector.h ..\src\GLEXT/GLVar.h ..\src\GLW/GLWidget.h ..\src\engine\FrameTimer.cpp ..\src\common/OptionEntry.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFontBanner.h ..\src\engine/MainLoopI.h ..\src\client/OptionsClient.h ..\src\engine/GameState.h ..\src\engine/FrameTimer.h ..\src\client/MainBanner.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\landscape/PatchVar.h
..\src\scorched\release\srclandscapeHeightMapscorched.obj: ..\src\common/Defines.h ..\src\landscape/HeightMap.h ..\src\common/Vector.h ..\src\common/Triangle.h ..\src\common/ProgressCounter.h ..\src\common/Line.h ..\src\landscape\HeightMap.cpp
..\src\scorched\release\srccomsComsMessageSenderscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\coms/NetServer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\coms/NetClient.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\coms\ComsMessageSender.cpp ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsMessageSender.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\src3dsparseMeshLODVectorscorched.obj: ..\src\common/Defines.h ..\src\3dsparse/MeshLODTri.h ..\src\common/Vector.h ..\src\3dsparse/MeshLODVector.h ..\src\3dsparse\MeshLODVector.cpp
..\src\scorched\release\srcactionsShieldHitscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\common/SoundStore.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\actions/ShieldHit.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\actions\ShieldHit.cpp ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\common/Sound.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\common/SoundBuffer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcGLEXTGLVertexArrayscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLStateExtension.h ..\src\GLEXT/GLVertexArray.h ..\src\GLEXT/GLState.h ..\src\GLEXT\GLVertexArray.cpp
..\src\scorched\release\srctankTankControllerscorched.obj: ..\src\engine/ActionMeta.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\actions/TankDamage.h ..\src\engine/ActionBuffer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\tank/TankController.h ..\src\common/Vector.h ..\src\tank\TankController.cpp ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srclandscapeSurroundDefsscorched.obj: ..\src\common/Defines.h ..\src\landscape/HeightMap.h ..\src\common/Vector.h ..\src\common/ProgressCounter.h ..\src\landscape/SurroundDefs.h ..\src\landscape\SurroundDefs.cpp
..\src\scorched\release\srcGLEXTGLFont2dscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLCameraFrustum.h ..\src\common/Vector.h ..\src\GLEXT\GLFont2d.cpp ..\src\GLEXT/GLTexture.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\GLEXT/GLFont2d.h ..\src\engine/GameState.h ..\src\GLEXT/GLBitmap.h ..\src\engine/GameStateI.h
..\src\scorched\release\srccommonFacescorched.obj: ..\src\common/Vector.h ..\src\common\Face.cpp ..\src\common/Face.h
..\src\scorched\release\srccomsComsMessagescorched.obj: ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms/NetBuffer.h ..\src\coms\ComsMessage.cpp
..\src\scorched\release\srcodearrayscorched.obj: ..\src\ode/memory.h ..\src\ode\array.cpp ..\src\ode/config.h ..\src\ode/error.h
..\src\scorched\release\srcactionsTankDeadscorched.obj: ..\src\common/SoundStore.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\common/OptionsGame.h ..\src\tank/TankBatteries.h ..\src\common/ARGParser.h ..\src\engine/PhysicsParticleObject.h ..\src\weapons/Weapon.h ..\src\engine/ActionBuffer.h ..\src\actions/ShotProjectile.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\actions/ShotProjectileExplosion.h ..\src\actions\TankDead.cpp ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\sprites/TankDeadRenderer.h ..\src\common/OptionEntry.h ..\src\common/Sound.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\common/SoundBuffer.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\actions/TankScored.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\actions/TankDead.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\actions/ShotProjectileFunky.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\actions/Explosion.h ..\src\engine/GameStateI.h ..\src\weapons/AccessoryStore.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\src3dsparseaseFile.tabscorched.obj: ..\src\common/Vector.h ..\src\3dsparse\aseFile.tab.cpp ..\src\3dsparse/ASEFile.h ..\src\3dsparse/ASEModel.h ..\src\common/Face.h
..\src\scorched\release\srcdialogsProgressDialogscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\common/Vector.h ..\src\GLW/GLWLabel.h ..\src\GLW/GLWWindow.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLW/GLWProgress.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\engine/MainLoop.h ..\src\dialogs/ProgressDialog.h ..\src\common/Timer.h ..\src\dialogs/BackdropDialog.h ..\src\GLW/GLWVisibleWidget.h ..\src\dialogs\ProgressDialog.cpp ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcactionsTankScoredscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\GLW/GLWidget.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFontBanner.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\actions/TankScored.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\sprites/TextActionRenderer.h ..\src\client/MainBanner.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h ..\src\actions\TankScored.cpp
..\src\scorched\release\srczlibinffastscorched.obj: ..\src\zlib\inffast.c
..\src\scorched\release\srclandscapeWaterMapModifierscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\landscape/WaterMapModifier.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\landscape/HeightMap.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\landscape/WaterMap.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\landscape/WaterMapSurround.h ..\src\landscape\WaterMapModifier.cpp ..\src\GLEXT/GLBitmap.h
..\src\scorched\release\srcserverServerPlayedMoveHandlerscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\server\ServerPlayedMoveHandler.cpp ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\coms/ComsMessageHandler.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\coms/ComsPlayedMoveMessage.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\server/ServerPlayedMoveHandler.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\server/ServerState.h ..\src\server/ServerShotHolder.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srcenginePhysicsEnginescorched.obj: ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\ode/config.h ..\src\ode/odecpp_collision.h ..\src\engine\PhysicsEngine.cpp ..\src\ode/odemath.h ..\src\ode/odecpp.h
..\src\scorched\release\srcdialogsScoreDialogscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\dialogs/ScoreDialog.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\GLW/GLWWindow.h ..\src\ode/collision_space.h ..\src\GLEXT/GLTexture.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\GLW/GLWidget.h ..\src\tank/TankSort.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\GLW/GLWVisiblePanel.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\GLEXT/GLFont2d.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\GLW/GLWVisibleWidget.h ..\src\dialogs\ScoreDialog.cpp ..\src\GLW/GLWFont.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcweaponsWeaponRiotBombscorched.obj: ..\src\engine/ActionMeta.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\weapons\WeaponRiotBomb.cpp ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\engine/PhysicsParticleObject.h ..\src\weapons/Weapon.h ..\src\weapons/WeaponRiotBomb.h ..\src\actions/ShotProjectile.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\actions/ShotProjectileRiot.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcserverScorchedASEscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\common/OptionsGame.h ..\src\server/OptionsServer.h ..\src\common/ARGParser.h ..\src\server\ScorchedASE.cpp ..\src\weapons/Weapon.h ..\src\common/FileLines.h ..\src\coms/NetServer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\server/ServerState.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srctankTankBatteriesscorched.obj: ..\src\common/Defines.h ..\src\tank/TankShields.h ..\src\tank/TankBatteries.h ..\src\common/Vector.h ..\src\weapons/Accessory.h ..\src\coms/NetBuffer.h ..\src\weapons/Battery.h ..\src\weapons/Shield.h ..\src\tank\TankBatteries.cpp
..\src\scorched\release\srcengineMainLoopscorched.obj: ..\src\common/Defines.h ..\src\common/ARGParser.h ..\src\common/FileLines.h ..\src\common/Vector.h ..\src\engine\MainLoop.cpp ..\src\common/OptionEntry.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\engine/MainLoop.h ..\src\client/OptionsClient.h ..\src\common/Timer.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srczlibuncomprscorched.obj: ..\src\zlib\uncompr.c
..\src\scorched\release\srcodestepscorched.obj: ..\src\ode/common.h ..\src\ode/rotation.h ..\src\ode/timer.h ..\src\ode/odemath.h ..\src\ode/config.h ..\src\ode/matrix.h ..\src\ode/error.h ..\src\ode\step.cpp
..\src\scorched\release\srccommonRandomGeneratorscorched.obj: ..\src\common/Defines.h ..\src\common\RandomGenerator.cpp ..\src\common/RandomGenerator.h
..\src\scorched\release\srccomsComsScoreMessagescorched.obj: ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms\ComsScoreMessage.cpp ..\src\coms/ComsScoreMessage.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srcGLEXTGLConsoleRulesscorched.obj: ..\src\GLEXT/GLConsoleRules.h ..\src\GLEXT\GLConsoleRules.cpp ..\src\GLEXT/GLConsoleRule.h
..\src\scorched\release\srcGLEXTGLBitmapModifierscorched.obj: ..\src\GLEXT/GLBitmapModifier.h ..\src\common/Defines.h ..\src\GLEXT/GLBitmapItterator.h ..\src\GLEXT/GLImage.h ..\src\landscape/HeightMap.h ..\src\common/Vector.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\GLEXT\GLBitmapModifier.cpp ..\src\GLEXT/GLBitmap.h ..\src\landscape/DeformLandscape.h
..\src\scorched\release\srccommonFileLinesscorched.obj: ..\src\common\FileLines.cpp ..\src\common/FileLines.h
..\src\scorched\release\srcactionsShotProjectileClodscorched.obj: ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\actions\ShotProjectileClod.cpp ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\engine/PhysicsParticleObject.h ..\src\actions/ShotProjectileClod.h ..\src\weapons/Weapon.h ..\src\engine/ActionBuffer.h ..\src\actions/ShotProjectile.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\engine/ScorchedCollisionIds.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\weapons/Accessory.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\coms/NetBuffer.h ..\src\actions/Explosion.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcengineGameStateIscorched.obj: ..\src\common/Vector.h ..\src\engine\GameStateI.cpp ..\src\common/KeyboardHistory.h ..\src\engine/MainLoopI.h ..\src\engine/GameState.h ..\src\engine/GameStateI.h
..\src\scorched\release\srctankTankStatescorched.obj: ..\src\tank\TankState.cpp ..\src\tank/TankState.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srcactionsShotProjectileFunkyscorched.obj: ..\src\engine/ActionMeta.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\engine/PhysicsParticleObject.h ..\src\weapons/Weapon.h ..\src\engine/ActionBuffer.h ..\src\actions/ShotProjectile.h ..\src\ode/error.h ..\src\actions/ShotProjectileExplosion.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\engine/MainLoopI.h ..\src\tank/TankLib.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\actions\ShotProjectileFunky.cpp ..\src\engine/ActionController.h ..\src\actions/ShotProjectileFunky.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srccommonTimerscorched.obj: ..\src\common/Defines.h ..\src\common\Timer.cpp ..\src\common/Timer.h
..\src\scorched\release\srctankgraphTankModelscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\common/Vector.h ..\src\tankgraph\TankModel.cpp ..\src\tankgraph/TankModelStore.h ..\src\GLEXT/GLTexture.h ..\src\GLEXT/GLVertexArray.h ..\src\tank/TankModelId.h ..\src\GLEXT/GLState.h ..\src\tankgraph/TankMesh.h ..\src\tankgraph/TankModel.h
..\src\scorched\release\srcodelcpscorched.obj: ..\src\ode/timer.h ..\src\ode/common.h ..\src\ode\lcp.cpp ..\src\ode/config.h ..\src\ode/matrix.h ..\src\ode/misc.h ..\src\ode/error.h
..\src\scorched\release\src3dsparseASEArrayFactscorched.obj: ..\src\common/Defines.h ..\src\common/Vector.h ..\src\GLEXT/GLVertexArray.h ..\src\GLEXT/GLState.h ..\src\3dsparse\ASEArrayFact.cpp ..\src\3dsparse/ASEArrayFact.h ..\src\GLEXT/GLVertexTexArray.h ..\src\3dsparse/ASEModel.h ..\src\common/Face.h
..\src\scorched\release\srcengineGameStatescorched.obj: ..\src\common/Defines.h ..\src\common/Vector.h ..\src\engine/GameStateStimulusI.h ..\src\engine\GameState.cpp ..\src\common/KeyboardHistory.h ..\src\engine/MainLoopI.h ..\src\engine/GameState.h ..\src\common/Keyboard.h ..\src\engine/GameStateI.h
..\src\scorched\release\srcGLWGLWScrollWscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWButton.h ..\src\GLW\GLWScrollW.cpp ..\src\GLW/GLWScrollButton.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLW/GLWScrollW.h ..\src\GLW/GLWVisibleWidget.h
..\src\scorched\release\srcspritesTracerActionRendererscorched.obj: ..\src\tank/TankShields.h ..\src\engine/ActionMeta.h ..\src\common/Defines.h ..\src\sprites\TracerActionRenderer.cpp ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\common/ARGParser.h ..\src\engine/PhysicsParticleObject.h ..\src\actions/ShotProjectileTracer.h ..\src\sprites/TracerActionRenderer.h ..\src\weapons/Weapon.h ..\src\actions/ShotProjectile.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\GLEXT/GLTexture.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\client/OptionsClient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\GLEXT/GLLenseFlare.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcGLEXTGLConsoleFileReaderscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\common/FileLines.h ..\src\GLEXT/GLConsoleRules.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\XML/XMLFile.h ..\src\GLEXT/GLConsoleRuleFn.h ..\src\XML/expat.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLConsole.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLConsoleRuleMethod.h ..\src\engine/MainLoopI.h ..\src\GLEXT/GLFont2d.h ..\src\engine/GameState.h ..\src\GLEXT/GLConsoleRule.h ..\src\GLEXT\GLConsoleFileReader.cpp ..\src\GLEXT/GLConsoleMethods.h ..\src\XML/XMLParser.h ..\src\engine/GameStateI.h ..\src\GLEXT/GLConsoleLines.h ..\src\GLEXT/GLConsoleFileReader.h
..\src\scorched\release\srctankaiTankAIComputerChooserscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\tankai/TankAIComputer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\tankai/TankAIComputerTosser.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tankai/TankAIComputerShooter.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tankai\TankAIComputerChooser.cpp ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\tankai/TankAIComputerBuyer.h ..\src\coms/NetBuffer.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\tankai/TankAIComputerChooser.h
..\src\scorched\release\srcserverServerPlayerReadyHandlerscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\server/ServerPlayerReadyHandler.h ..\src\weapons/Weapon.h ..\src\coms/ComsPlayerReadyMessage.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\coms/ComsMessageHandler.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\server\ServerPlayerReadyHandler.cpp ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\server/ServerState.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\server/ServerDialog.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srcserverServerShotStatescorched.obj: ..\src\engine/ActionMeta.h ..\src\coms/ComsActionsMessage.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\common/OptionsGame.h ..\src\tank/TankBatteries.h ..\src\common/ARGParser.h ..\src\weapons/Weapon.h ..\src\engine/ActionBuffer.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\tank/TankStart.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\engine/GameStateStimulusI.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\coms/ComsPlayedMoveMessage.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\server/ServerShotState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsMessageSender.h ..\src\server/ServerState.h ..\src\server/ServerShotHolder.h ..\src\engine/ActionController.h ..\src\server\ServerShotState.cpp ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\server/ServerDialog.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srclandscapePatchGridscorched.obj: ..\src\tank/TankShields.h ..\src\GLEXT/GLCamera.h ..\src\client/MainCamera.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\landscape/Variance.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLCameraFrustum.h ..\src\weapons/Weapon.h ..\src\landscape/PatchGrid.h ..\src\landscape/HeightMap.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\GLEXT/GLVar.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\landscape/TriNode.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\landscape/TriNodePool.h ..\src\ode/config.h ..\src\landscape\PatchGrid.cpp ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\landscape/Patch.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\common/Line.h ..\src\ode/odecpp.h ..\src\landscape/PatchSide.h ..\src\landscape/PatchVar.h
..\src\scorched\release\srclandscapeHeightMapSurroundscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\landscape/HeightMap.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\landscape\HeightMapSurround.cpp ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\landscape/SurroundDefs.h ..\src\GLEXT/GLBitmap.h ..\src\landscape/HeightMapSurround.h
..\src\scorched\release\srctankaiTankAIHumanCtrlscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\tankai\TankAIHumanCtrl.cpp ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tankai/TankAIHumanCtrl.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\client/ClientState.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srczlibinfblockscorched.obj: ..\src\zlib\infblock.c
..\src\scorched\release\srcGLWGLWCheckBoxscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWCheckBox.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLW\GLWCheckBox.cpp
..\src\scorched\release\srcweaponsParachutescorched.obj: ..\src\common/Defines.h ..\src\weapons\Parachute.cpp ..\src\weapons/Parachute.h ..\src\weapons/Accessory.h
..\src\scorched\release\src3dsparseASEFilescorched.obj: ..\src\common/Defines.h ..\src\common/Vector.h ..\src\3dsparse/ASEFile.h ..\src\3dsparse\ASEFile.cpp ..\src\3dsparse/ASEModel.h ..\src\common/Face.h
..\src\scorched\release\srcodefastdotscorched.obj: ..\src\ode\fastdot.c
..\src\scorched\release\srctankTankStartscorched.obj: ..\src\engine/ActionMeta.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\common/OptionsGame.h ..\src\tank/TankBatteries.h ..\src\common/ARGParser.h ..\src\weapons/Weapon.h ..\src\landscape/HeightMap.h ..\src\engine/ActionBuffer.h ..\src\landscape/GlobalHMap.h ..\src\tank\TankStart.cpp ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\tank/TankStart.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\coms/ComsLevelMessage.h ..\src\tank/TankContainer.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\actions/TankScored.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\common/RandomGenerator.h ..\src\engine/ActionController.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcserverServerTextHandlerscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\coms/ComsTextMessage.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\server\ServerTextHandler.cpp ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\coms/ComsMessageHandler.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsMessageSender.h ..\src\tank/TankAutoDefense.h ..\src\server/ServerTextHandler.h ..\src\coms/NetBuffer.h ..\src\server/ServerDialog.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srctankaiTankAIComputerTosserscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\tankai/TankAIComputer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\tankai/TankAIComputerTosser.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankLib.h ..\src\ode/timer.h ..\src\tankai/TankAIComputerShooter.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\tankai/TankAIComputerBuyer.h ..\src\coms/NetBuffer.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\tankai\TankAIComputerTosser.cpp
..\src\scorched\release\srcweaponsWeaponMirvscorched.obj: ..\src\engine/ActionMeta.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\weapons\WeaponMirv.cpp ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\engine/PhysicsParticleObject.h ..\src\weapons/Weapon.h ..\src\actions/ShotProjectile.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\weapons/WeaponMirv.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\actions/ShotProjectileMirv.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcspritesExplosionTexturesscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLTextureSet.h ..\src\GLEXT/GLImage.h ..\src\sprites/ExplosionTextures.h ..\src\GLEXT/GLTexture.h ..\src\sprites\ExplosionTextures.cpp ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\GLEXT/GLBitmap.h
..\src\scorched\release\srclandscapePatchscorched.obj: ..\src\common/Defines.h ..\src\landscape/Variance.h ..\src\landscape/HeightMap.h ..\src\common/Vector.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\landscape/TriNode.h ..\src\landscape/TriNodePool.h ..\src\landscape/Patch.h ..\src\landscape\Patch.cpp ..\src\landscape/PatchSide.h
..\src\scorched\release\srcGLWGLWProgressscorched.obj: ..\src\common/Defines.h ..\src\GLW\GLWProgress.cpp ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLW/GLWProgress.h ..\src\GLEXT/GLState.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcGLWGLWPushButtonscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWPushButton.h ..\src\GLW/GLWButton.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLW\GLWPushButton.cpp
..\src\scorched\release\srcactionsTankSayscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\weapons/Weapon.h ..\src\actions/TankSay.h ..\src\sprites/TalkRenderer.h ..\src\engine/ActionBuffer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\GLEXT/GLTexture.h ..\src\ode/odecpp_old_collision.h ..\src\GLW/GLWidget.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFontBanner.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\actions\TankSay.cpp ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\client/MainBanner.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcserverServerNextRoundStatescorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\common/OptionsGame.h ..\src\common/ARGParser.h ..\src\weapons/Weapon.h ..\src\engine/ActionBuffer.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\tank/TankStart.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\engine/GameStateStimulusI.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\server/ServerNextRoundState.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\server/ServerState.h ..\src\engine/ActionController.h ..\src\server\ServerNextRoundState.cpp ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\server/ServerDialog.h ..\src\engine/GameStateI.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\server/ServerNewGameState.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srclandscapeDeformTexturesscorched.obj: ..\src\GLEXT/GLBitmapModifier.h ..\src\common/Defines.h ..\src\landscape/Variance.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\landscape/ShadowMap.h ..\src\landscape/PatchGrid.h ..\src\landscape/HeightMap.h ..\src\landscape/GlobalHMap.h ..\src\landscape/Surround.h ..\src\common/Vector.h ..\src\landscape/Wall.h ..\src\landscape/WaterMapPoints.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\GLEXT/GLTexture.h ..\src\landscape/Smoke.h ..\src\landscape/Landscape.h ..\src\landscape/WaterMap.h ..\src\common/KeyboardHistory.h ..\src\coms/ComsLevelMessage.h ..\src\landscape/SmokeChain.h ..\src\landscape/Sun.h ..\src\landscape\DeformTextures.cpp ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\landscape/TriNode.h ..\src\landscape/TriNodePool.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\landscape/Patch.h ..\src\landscape/WaterMapSurround.h ..\src\landscape/SurroundDefs.h ..\src\GLEXT/GLBitmap.h ..\src\landscape/HeightMapSurround.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\landscape/DeformTextures.h ..\src\landscape/DeformLandscape.h ..\src\landscape/PatchSide.h
..\src\scorched\release\srcdialogsConnectDialogscorched.obj: ..\src\common/Defines.h ..\src\common/ARGParser.h ..\src\GLW/GLWButton.h ..\src\GLW/GLWTextBox.h ..\src\client/ClientActionsHandler.h ..\src\common/FileLines.h ..\src\dialogs\ConnectDialog.cpp ..\src\common/Vector.h ..\src\GLW/GLWLabel.h ..\src\client/ClientStartGameHandler.h ..\src\GLW/GLWListView.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\dialogs/PlayerDialog.h ..\src\coms/NetClient.h ..\src\GLW/GLWWindow.h ..\src\coms/NetMessageHandler.h ..\src\client/ClientRmPlayerHandler.h ..\src\client/ClientGameStoppedHandler.h ..\src\GLW/GLWScrollButton.h ..\src\dialogs/ConnectDialog.h ..\src\GLW/GLWidget.h ..\src\tankgraph/GLWTankViewer.h ..\src\dialogs/QuitDialog.h ..\src\GLEXT/GLVertexArray.h ..\src\common/OptionEntry.h ..\src\coms/ComsMessageHandler.h ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\common/WindowManager.h ..\src\GLW/GLWTextButton.h ..\src\client/ClientScoreHandler.h ..\src\GLEXT/GLState.h ..\src\client/ClientAddPlayerHandler.h ..\src\engine/MainLoopI.h ..\src\client/ClientTextHandler.h ..\src\engine/MainLoop.h ..\src\client/OptionsClient.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\coms/ComsConnectMessage.h ..\src\GLW/GLWScrollW.h ..\src\common/Timer.h ..\src\client/ClientBuyAccessoryHandler.h ..\src\coms/ComsMessageSender.h ..\src\GLW/GLWVisibleWidget.h ..\src\client/ClientMessageHandler.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\client/ClientDefenseHandler.h ..\src\client/ClientConnectionAcceptHandler.h ..\src\client/ClientNewGameHandler.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcengineActionMetascorched.obj: ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\engine\ActionMeta.cpp ..\src\coms/NetBuffer.h ..\src\engine/Action.h
..\src\scorched\release\srctankaiTankAIStorescorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\tankai\TankAIStore.cpp ..\src\weapons/Weapon.h ..\src\tankai/TankAIComputerDHTest.h ..\src\tankai/TankAIComputer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\tankai/TankAIComputerRandom.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\tankai/TankAIComputerTosser.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tankai/TankAIComputerPShark.h ..\src\ode/timer.h ..\src\tankai/TankAIComputerShooter.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\tankai/TankAIStore.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\tankai/TankAIComputerBuyer.h ..\src\coms/NetBuffer.h ..\src\tankai/TankAIComputerMoron.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\tankai/TankAIComputerChooser.h
..\src\scorched\release\srcactionsTankDamagescorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\engine/PhysicsParticleObject.h ..\src\weapons/Weapon.h ..\src\actions/TankDamage.h ..\src\landscape/HeightMap.h ..\src\landscape/GlobalHMap.h ..\src\engine/ActionBuffer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\actions/TankFalling.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\coms/ComsLevelMessage.h ..\src\tank/TankContainer.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\actions/TankDead.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\actions\TankDamage.cpp ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srccommonKeyboardscorched.obj: ..\src\common/KeyboardHistory.h ..\src\common/Keyboard.h ..\src\common\Keyboard.cpp
..\src\scorched\release\srczlibtreesscorched.obj: ..\src\zlib\trees.c
..\src\scorched\release\srcXMLxmlparsescorched.obj: ..\src\XML/expat_config.h ..\src\XML\xmlparse.c
..\src\scorched\release\srclandscapeSunscorched.obj: ..\src\common/Defines.h ..\src\common/OptionsGame.h ..\src\GLEXT/GLImage.h ..\src\common/ARGParser.h ..\src\GLEXT/GLCameraFrustum.h ..\src\common/FileLines.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\common/OptionEntry.h ..\src\common/KeyboardHistory.h ..\src\landscape/Sun.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\client/OptionsClient.h ..\src\engine/GameState.h ..\src\landscape\Sun.cpp ..\src\GLEXT/GLBitmap.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h
..\src\scorched\release\srccommonVectorscorched.obj: ..\src\common/Defines.h ..\src\common/Vector.h ..\src\common\Vector.cpp
..\src\scorched\release\srctankgraphTankModelStorescorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\common/ARGParser.h ..\src\common/FileLines.h ..\src\common/Vector.h ..\src\tankgraph/TankModelStore.h ..\src\GLEXT/GLTexture.h ..\src\GLEXT/GLVertexArray.h ..\src\XML/XMLFile.h ..\src\3dsparse/ASEFile.h ..\src\common/OptionEntry.h ..\src\XML/expat.h ..\src\tank/TankModelId.h ..\src\GLEXT/GLState.h ..\src\tankgraph\TankModelStore.cpp ..\src\client/OptionsClient.h ..\src\tankgraph/TankMesh.h ..\src\XML/XMLParser.h ..\src\GLEXT/GLBitmap.h ..\src\coms/NetBuffer.h ..\src\tankgraph/TankModel.h ..\src\3dsparse/ASEModel.h ..\src\common/Face.h
..\src\scorched\release\srclandscapeSmokescorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLCameraFrustum.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\landscape/Smoke.h ..\src\common/KeyboardHistory.h ..\src\landscape/SmokeChain.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\engine/GameState.h ..\src\GLEXT/GLBitmap.h ..\src\engine/GameStateI.h ..\src\landscape\Smoke.cpp
..\src\scorched\release\srcclientClientNextRoundStatescorched.obj: ..\src\coms/ComsPlayerReadyMessage.h ..\src\client/ClientNextRoundState.h ..\src\common/Vector.h ..\src\client/ClientStartGameHandler.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms/NetMessageHandler.h ..\src\coms/ComsMessageHandler.h ..\src\common/KeyboardHistory.h ..\src\engine/MainLoopI.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\client\ClientNextRoundState.cpp ..\src\coms/ComsMessageSender.h ..\src\client/ClientState.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h
..\src\scorched\release\srctankgraphTankRendererscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\weapons/Weapon.h ..\src\tankgraph/TankMenus.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\GLEXT/GLTexture.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\GLEXT/GLVertexArray.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tankgraph/TracerStore.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\tankgraph/TankMesh.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tankgraph\TankRenderer.cpp ..\src\tankgraph/TankModelRenderer.h ..\src\client/ClientState.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\tankgraph/TankModel.h ..\src\engine/GameStateI.h ..\src\tankgraph/TankRenderer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srcclientClientBuyAccessoryHandlerscorched.obj: ..\src\common/SoundStore.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\client\ClientBuyAccessoryHandler.cpp ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\coms/ComsMessageHandler.h ..\src\common/Sound.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\common/SoundBuffer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\client/ClientBuyAccessoryHandler.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsBuyAccessoryMessage.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srccomsComsBuyAccessoryMessagescorched.obj: ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms\ComsBuyAccessoryMessage.cpp ..\src\coms/ComsBuyAccessoryMessage.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srcGLEXTGLConsoleMethodsscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLConsoleRules.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLConsoleRuleMethod.h ..\src\GLEXT/GLFont2d.h ..\src\GLEXT\GLConsoleMethods.cpp ..\src\GLEXT/GLConsoleRule.h ..\src\GLEXT/GLConsoleMethods.h ..\src\GLEXT/GLConsoleLines.h ..\src\GLEXT/GLConsoleFileReader.h
..\src\scorched\release\srctankTankShieldsscorched.obj: ..\src\tank/TankShields.h ..\src\common/Vector.h ..\src\weapons/Accessory.h ..\src\tank\TankShields.cpp ..\src\coms/NetBuffer.h ..\src\weapons/AccessoryStore.h ..\src\weapons/Shield.h
..\src\scorched\release\srccommonWindowManagerscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\common/Vector.h ..\src\GLW/GLWWindow.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLMenu.h ..\src\GLW/GLWVisiblePanel.h ..\src\common/WindowManager.h ..\src\GLEXT/GLState.h ..\src\dialogs/MainMenuDialog.h ..\src\engine/MainLoopI.h ..\src\GLEXT/GLFont2d.h ..\src\common\WindowManager.cpp ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\common/Keyboard.h ..\src\GLW/GLWVisibleWidget.h ..\src\engine/GameStateI.h ..\src\GLEXT/GLMenuEntry.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srccommonSoundStorescorched.obj: ..\src\common/Defines.h ..\src\common/SoundStore.h ..\src\common\SoundStore.cpp ..\src\common/Sound.h ..\src\common/SoundBuffer.h
..\src\scorched\release\srcodetestingscorched.obj: ..\src\ode/common.h ..\src\ode/memory.h ..\src\ode/misc.h ..\src\ode/config.h ..\src\ode\testing.cpp ..\src\ode/error.h
..\src\scorched\release\srcclientClientDialogscorched.obj: ..\src\tank/TankShields.h ..\src\engine/ActionMeta.h ..\src\GLEXT/GLCamera.h ..\src\client/MainCamera.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\common/ARGParser.h ..\src\GLEXT/GLStateExtension.h ..\src\weapons/Weapon.h ..\src\client/GLSetup.h ..\src\landscape/HeightMap.h ..\src\landscape/GlobalHMap.h ..\src\engine/ActionBuffer.h ..\src\common/FileLines.h ..\src\common/Display.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\engine/ScorchedCollisionHandler.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\client\ClientDialog.cpp ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\tankgraph/TankModelStore.h ..\src\GLEXT/GLTexture.h ..\src\landscape/HeightMapCollision.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\GLEXT/GLVertexArray.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\common/Sound.h ..\src\tank/TankWeapon.h ..\src\common/Gamma.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\client/WindowSetup.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\coms/ComsLevelMessage.h ..\src\client/ClientDialog.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\engine/MainLoop.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\client/OptionsClient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\GLEXT/GLMenuI.h ..\src\common/Keyboard.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tankgraph/TankMesh.h ..\src\tank/TankState.h ..\src\common/Timer.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\client/ClientState.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\tankgraph/TankModel.h ..\src\engine/GameStateI.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\common/Line.h ..\src\ode/odecpp.h ..\src\GLEXT/GLConsoleFileReader.h ..\src\engine/Action.h
..\src\scorched\release\srcXMLXMLParserscorched.obj: ..\src\common/Defines.h ..\src\XML\XMLParser.cpp ..\src\XML/expat.h ..\src\XML/XMLParser.h
..\src\scorched\release\srcGLEXTGLTextureSetscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLTextureSet.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLTexture.h ..\src\GLEXT/GLState.h ..\src\GLEXT\GLTextureSet.cpp
..\src\scorched\release\srccomsNetServerscorched.obj: ..\src\common/Defines.h ..\src\coms/NetServer.h ..\src\coms\NetServer.cpp ..\src\coms/NetMessagePool.h ..\src\coms/NetMessage.h ..\src\coms/NetMessageHandler.h ..\src\coms/NetBufferUtil.h ..\src\coms/NetBuffer.h
..\src\scorched\release\src3dsparseMeshLODTriscorched.obj: ..\src\common/Defines.h ..\src\3dsparse/MeshLODTri.h ..\src\3dsparse\MeshLODTri.cpp ..\src\common/Vector.h ..\src\3dsparse/MeshLODVector.h
..\src\scorched\release\srcspritesWallActionRendererscorched.obj: ..\src\common/Defines.h ..\src\landscape/Variance.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\landscape/ShadowMap.h ..\src\landscape/PatchGrid.h ..\src\landscape/HeightMap.h ..\src\landscape/GlobalHMap.h ..\src\landscape/Surround.h ..\src\common/Vector.h ..\src\landscape/Wall.h ..\src\landscape/WaterMapPoints.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\sprites\WallActionRenderer.cpp ..\src\GLEXT/GLTexture.h ..\src\landscape/Smoke.h ..\src\sprites/WallActionRenderer.h ..\src\landscape/Landscape.h ..\src\landscape/WaterMap.h ..\src\common/KeyboardHistory.h ..\src\coms/ComsLevelMessage.h ..\src\landscape/SmokeChain.h ..\src\landscape/Sun.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\landscape/TriNode.h ..\src\landscape/TriNodePool.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\landscape/Patch.h ..\src\landscape/WaterMapSurround.h ..\src\landscape/SurroundDefs.h ..\src\GLEXT/GLBitmap.h ..\src\landscape/HeightMapSurround.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\landscape/PatchSide.h ..\src\engine/Action.h
..\src\scorched\release\srcXMLxmltokscorched.obj: ..\src\XML\xmltok.c ..\src\XML/expat_config.h
..\src\scorched\release\srccomsComsPlayedMoveMessagescorched.obj: ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms/ComsPlayedMoveMessage.h ..\src\coms\ComsPlayedMoveMessage.cpp ..\src\coms/NetBuffer.h
..\src\scorched\release\srcodegeomscorched.obj: ..\src\ode/common.h ..\src\ode/memory.h ..\src\ode/objects.h ..\src\ode/geom.h ..\src\ode\geom.cpp ..\src\ode/odemath.h ..\src\ode/rotation.h ..\src\ode/mass.h ..\src\ode/config.h ..\src\ode/matrix.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/error.h
..\src\scorched\release\srcengineActionscorched.obj: ..\src\engine\Action.cpp ..\src\engine/Action.h
..\src\scorched\release\srcGLEXTGLConsoleRuleFnIAdapterscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\common/ARGParser.h ..\src\common/FileLines.h ..\src\GLEXT/GLConsoleRules.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\GLEXT/GLConsoleRuleFn.h ..\src\common/OptionEntry.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLConsole.h ..\src\GLEXT/GLState.h ..\src\GLEXT\GLConsoleRuleFnIAdapter.cpp ..\src\GLEXT/GLConsoleRuleMethod.h ..\src\engine/MainLoopI.h ..\src\GLEXT/GLFont2d.h ..\src\GLEXT/GLConsoleRuleFnIAdapter.h ..\src\engine/GameState.h ..\src\GLEXT/GLConsoleRule.h ..\src\GLEXT/GLConsoleMethods.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\GLEXT/GLConsoleLines.h
..\src\scorched\release\srcserverServerWaitingForPlayersStatescorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\common/OptionsGame.h ..\src\tank/TankBatteries.h ..\src\common/ARGParser.h ..\src\weapons/Weapon.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\engine/GameStateStimulusI.h ..\src\server/ServerWaitingForPlayersState.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\server/ServerDialog.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\server\ServerWaitingForPlayersState.cpp
..\src\scorched\release\srcGLEXTGLTextureCubeMapscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\GLEXT/GLTexture.h ..\src\GLEXT\GLTextureCubeMap.cpp ..\src\GLEXT/GLState.h
..\src\scorched\release\srccomsComsConnectAcceptMessagescorched.obj: ..\src\common/OptionsGame.h ..\src\common/ARGParser.h ..\src\common/FileLines.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms/ComsConnectAcceptMessage.h ..\src\common/OptionEntry.h ..\src\coms\ComsConnectAcceptMessage.cpp ..\src\coms/NetBuffer.h
..\src\scorched\release\srcclientClientActionsHandlerscorched.obj: ..\src\engine/ActionMeta.h ..\src\coms/ComsActionsMessage.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\engine/ActionBuffer.h ..\src\client/ClientActionsHandler.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\coms/ComsMessageHandler.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\client\ClientActionsHandler.cpp ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\client/ClientState.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcserverServerPlayingStatescorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\common/OptionsGame.h ..\src\common/ARGParser.h ..\src\weapons/Weapon.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\engine/GameStateStimulusI.h ..\src\ode/odecpp_old_collision.h ..\src\server/ServerPlayingState.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\coms/ComsPlayedMoveMessage.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\server\ServerPlayingState.cpp ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\coms/ComsStartGameMessage.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsMessageSender.h ..\src\server/ServerShotHolder.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\server/ServerDialog.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\coms/ComsPlayerStateMessage.h ..\src\ode/odecpp.h
..\src\scorched\release\srccomsComsActionsMessagescorched.obj: ..\src\engine/ActionMeta.h ..\src\coms/ComsActionsMessage.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\engine/ActionBuffer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\coms\ComsActionsMessage.cpp ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srctankTankColorGeneratorscorched.obj: ..\src\common/Vector.h ..\src\tank\TankColorGenerator.cpp ..\src\tank/TankColorGenerator.h
..\src\scorched\release\srcweaponsWeaponClodscorched.obj: ..\src\engine/ActionMeta.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\engine/PhysicsParticleObject.h ..\src\actions/ShotProjectileClod.h ..\src\weapons/Weapon.h ..\src\actions/ShotProjectile.h ..\src\weapons\WeaponClod.cpp ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\weapons/WeaponClod.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srclandscapeShadowMapscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\landscape/ShadowMap.h ..\src\GLEXT/GLTexture.h ..\src\landscape\ShadowMap.cpp ..\src\GLEXT/GLState.h
..\src\scorched\release\srcodeodemathscorched.obj: ..\src\ode/common.h ..\src\ode\odemath.cpp ..\src\ode/odemath.h ..\src\ode/config.h ..\src\ode/error.h
..\src\scorched\release\srcdialogsKibitzingDialogscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWButton.h ..\src\dialogs/KibitzingDialog.h ..\src\common/Vector.h ..\src\GLW/GLWLabel.h ..\src\GLW/GLWWindow.h ..\src\dialogs\KibitzingDialog.cpp ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\common/WindowManager.h ..\src\GLW/GLWTextButton.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\GLW/GLWVisibleWidget.h ..\src\engine/GameStateI.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\src3dsparseASEModelscorched.obj: ..\src\3dsparse/MeshLOD.h ..\src\common/Defines.h ..\src\common/Vector.h ..\src\GLEXT/GLState.h ..\src\3dsparse\ASEModel.cpp ..\src\3dsparse/ASEModel.h ..\src\common/Face.h
..\src\scorched\release\srcengineScorchedCollisionIdsscorched.obj: ..\src\engine\ScorchedCollisionIds.cpp ..\src\engine/ScorchedCollisionIds.h
..\src\scorched\release\srcGLEXTGLLenseFlarescorched.obj: ..\src\GLEXT/GLCamera.h ..\src\client/MainCamera.h ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLCameraFrustum.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\engine/GameState.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameStateI.h ..\src\GLEXT\GLLenseFlare.cpp ..\src\GLEXT/GLLenseFlare.h ..\src\common/Line.h ..\src\GLEXT/GLLuminance.h
..\src\scorched\release\srccomsComsLevelMessagescorched.obj: ..\src\common/Defines.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms/ComsLevelMessage.h ..\src\coms\ComsLevelMessage.cpp ..\src\coms/NetBuffer.h
..\src\scorched\release\srcGLEXTGLStatescorched.obj: ..\src\common/Defines.h ..\src\GLEXT\GLState.cpp ..\src\GLEXT/GLState.h
..\src\scorched\release\srctankaiTankAIComputerShooterscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\tankai/TankAIComputer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\tankai\TankAIComputerShooter.cpp ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankLib.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tankai/TankAIComputerShooter.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\tankai/TankAIComputerBuyer.h ..\src\coms/NetBuffer.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srccommonSoundscorched.obj: ..\src\common/Sound.h ..\src\common/SoundBuffer.h ..\src\common\Sound.cpp
..\src\scorched\release\srcspritesTextActionRendererscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\common/Vector.h ..\src\sprites\TextActionRenderer.cpp ..\src\GLEXT/GLTexture.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFont2d.h ..\src\sprites/TextActionRenderer.h ..\src\GLW/GLWFont.h ..\src\engine/Action.h
..\src\scorched\release\srcactionsShotProjectileExplosionscorched.obj: ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\engine/PhysicsParticleObject.h ..\src\weapons/Weapon.h ..\src\engine/ActionBuffer.h ..\src\actions/ShotProjectile.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\actions/ShotProjectileExplosion.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\engine/ScorchedCollisionIds.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\weapons/Accessory.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\coms/NetBuffer.h ..\src\actions/Explosion.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\actions\ShotProjectileExplosion.cpp ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srctankTankSortscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\tank/TankSort.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\tank\TankSort.cpp ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srccommonGammascorched.obj: ..\src\common/Defines.h ..\src\common\Gamma.cpp ..\src\common/Gamma.h
..\src\scorched\release\srcclientClientRmPlayerHandlerscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\GLW/GLWButton.h ..\src\GLW/GLWTextBox.h ..\src\coms/ComsRmPlayerMessage.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\GLW/GLWLabel.h ..\src\common/Vector.h ..\src\GLW/GLWListView.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\GLW/GLWWindow.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\client/ClientRmPlayerHandler.h ..\src\GLW/GLWScrollButton.h ..\src\dialogs/ConnectDialog.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\GLW/GLWidget.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\coms/ComsMessageHandler.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLW/GLWTextButton.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFontBanner.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\GLW/GLWScrollW.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\GLW/GLWVisibleWidget.h ..\src\client/ClientState.h ..\src\client/MainBanner.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\client\ClientRmPlayerHandler.cpp ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcclientClientAddPlayerHandlerscorched.obj: ..\src\coms/ComsAddPlayerMessage.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\GLW/GLWButton.h ..\src\GLW/GLWTextBox.h ..\src\common/FileLines.h ..\src\client\ClientAddPlayerHandler.cpp ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\GLW/GLWLabel.h ..\src\common/Vector.h ..\src\GLW/GLWListView.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\GLW/GLWWindow.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\GLW/GLWScrollButton.h ..\src\dialogs/ConnectDialog.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\GLW/GLWidget.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\coms/ComsMessageHandler.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLW/GLWTextButton.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFontBanner.h ..\src\engine/MainLoopI.h ..\src\client/ClientAddPlayerHandler.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\GLW/GLWScrollW.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\GLW/GLWVisibleWidget.h ..\src\client/ClientState.h ..\src\client/MainBanner.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\client/ClientConnectionAcceptHandler.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\tankai/TankAIHuman.h ..\src\ode/odecpp.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srclandscapeSmokeChainscorched.obj: ..\src\common/Defines.h ..\src\landscape/Variance.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\landscape/ShadowMap.h ..\src\landscape/PatchGrid.h ..\src\landscape/HeightMap.h ..\src\landscape/Surround.h ..\src\landscape/GlobalHMap.h ..\src\common/Vector.h ..\src\landscape/Wall.h ..\src\landscape/WaterMapPoints.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\GLEXT/GLTexture.h ..\src\landscape/Smoke.h ..\src\landscape/Landscape.h ..\src\landscape/WaterMap.h ..\src\common/KeyboardHistory.h ..\src\landscape/SmokeChain.h ..\src\coms/ComsLevelMessage.h ..\src\landscape/Sun.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\landscape\SmokeChain.cpp ..\src\engine/MainLoopI.h ..\src\landscape/TriNode.h ..\src\landscape/TriNodePool.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\landscape/Patch.h ..\src\landscape/WaterMapSurround.h ..\src\landscape/SurroundDefs.h ..\src\GLEXT/GLBitmap.h ..\src\landscape/HeightMapSurround.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\landscape/PatchSide.h
..\src\scorched\release\srcGLEXTGLMenuIscorched.obj: ..\src\GLEXT\GLMenuI.cpp ..\src\GLEXT/GLMenuI.h
..\src\scorched\release\srctankaiTankAIComputerPSharkscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\tankai\TankAIComputerPShark.cpp ..\src\weapons/Weapon.h ..\src\tankai/TankAIComputer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\tankai/TankAIComputerTosser.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tankai/TankAIComputerPShark.h ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tankai/TankAIComputerShooter.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\tankai/TankAIComputerBuyer.h ..\src\coms/NetBuffer.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\tankai/TankAIComputerChooser.h
..\src\scorched\release\srcodemassscorched.obj: ..\src\ode/common.h ..\src\ode/odemath.h ..\src\ode/mass.h ..\src\ode/config.h ..\src\ode/matrix.h ..\src\ode\mass.cpp ..\src\ode/error.h
..\src\scorched\release\srcGLWGLWLabelscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLW\GLWLabel.cpp ..\src\common/Vector.h ..\src\GLW/GLWLabel.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFont2d.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLW/GLWFont.h
..\src\scorched\release\srcserverServerConnectHandlerscorched.obj: ..\src\coms/ComsAddPlayerMessage.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\common/OptionsGame.h ..\src\tank/TankBatteries.h ..\src\common/ARGParser.h ..\src\server/OptionsServer.h ..\src\weapons/Weapon.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\server/ServerConnectHandler.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\coms/ComsConnectAcceptMessage.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\server\ServerConnectHandler.cpp ..\src\common/OptionEntry.h ..\src\coms/ComsMessageHandler.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\coms/ComsConnectMessage.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsMessageSender.h ..\src\server/ServerState.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\tank/TankColorGenerator.h ..\src\server/ServerDialog.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srcGLWGLWButtonscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWButton.h ..\src\GLW\GLWButton.cpp ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\common/Keyboard.h ..\src\GLW/GLWVisibleWidget.h
..\src\scorched\release\srccomsNetMessagescorched.obj: ..\src\coms/NetMessage.h ..\src\coms\NetMessage.cpp ..\src\coms/NetBuffer.h
..\src\scorched\release\srclandscapeTriNodePoolscorched.obj: ..\src\landscape/TriNode.h ..\src\landscape/TriNodePool.h ..\src\landscape\TriNodePool.cpp
..\src\scorched\release\srcodefastldltscorched.obj: ..\src\ode\fastldlt.c
..\src\scorched\release\srcscorchedCreateDialogsscorched.obj: ..\src\scorched/MainDialog.h ..\src\scorched\CreateDialogs.cpp
..\src\scorched\release\srccomsComsPlayerStateMessagescorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms\ComsPlayerStateMessage.cpp ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\coms/ComsPlayerStateMessage.h ..\src\ode/odecpp.h
..\src\scorched\release\srcclientOptionsClientscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\common/ARGParser.h ..\src\common/FileLines.h ..\src\GLEXT/GLConsoleRules.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\GLEXT/GLConsoleRuleFn.h ..\src\common/OptionEntry.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLConsole.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLConsoleRuleMethod.h ..\src\engine/MainLoopI.h ..\src\GLEXT/GLFont2d.h ..\src\GLEXT/GLConsoleRuleFnIAdapter.h ..\src\client/OptionsClient.h ..\src\engine/GameState.h ..\src\GLEXT/GLConsoleRule.h ..\src\GLEXT/GLConsoleMethods.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\GLEXT/GLConsoleLines.h ..\src\client\OptionsClient.cpp
..\src\scorched\release\srccommonDisplayscorched.obj: ..\src\common/Display.h ..\src\common\Display.cpp
..\src\scorched\release\srcGLEXTGLCamerascorched.obj: ..\src\GLEXT/GLCamera.h ..\src\common/Defines.h ..\src\GLEXT\GLCamera.cpp ..\src\common/Vector.h ..\src\GLEXT/GLState.h ..\src\common/Line.h
..\src\scorched\release\srccomsComsConnectMessagescorched.obj: ..\src\coms\ComsConnectMessage.cpp ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms/ComsConnectMessage.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srccomsComsTextMessagescorched.obj: ..\src\coms/ComsTextMessage.h ..\src\common/Vector.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms\ComsTextMessage.cpp ..\src\coms/NetBuffer.h
..\src\scorched\release\srcspritesTalkRendererscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLCameraFrustum.h ..\src\sprites/TalkRenderer.h ..\src\sprites\TalkRenderer.cpp ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\engine/GameState.h ..\src\GLEXT/GLBitmap.h ..\src\engine/GameStateI.h ..\src\engine/Action.h
..\src\scorched\release\srcdialogsPlanViewDialogscorched.obj: ..\src\GLEXT/GLCamera.h ..\src\client/MainCamera.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\dialogs/PlanViewDialog.h ..\src\landscape/Variance.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\weapons/Weapon.h ..\src\landscape/ShadowMap.h ..\src\landscape/PatchGrid.h ..\src\dialogs\PlanViewDialog.cpp ..\src\landscape/HeightMap.h ..\src\landscape/Surround.h ..\src\landscape/GlobalHMap.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\landscape/Wall.h ..\src\landscape/WaterMapPoints.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\GLW/GLWWindow.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\GLEXT/GLTexture.h ..\src\ode/odecpp_old_collision.h ..\src\GLW/GLWidget.h ..\src\ode/mass.h ..\src\landscape/Smoke.h ..\src\ode/matrix.h ..\src\landscape/Landscape.h ..\src\tank/TankParachutes.h ..\src\landscape/WaterMap.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\GLW/GLWVisiblePanel.h ..\src\landscape/SmokeChain.h ..\src\coms/ComsLevelMessage.h ..\src\landscape/Sun.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\landscape/TriNode.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\landscape/TriNodePool.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\GLEXT/GLMenuI.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\landscape/Patch.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\landscape/WaterMapSurround.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\GLW/GLWVisibleWidget.h ..\src\landscape/SurroundDefs.h ..\src\GLEXT/GLBitmap.h ..\src\tank/TankAutoDefense.h ..\src\landscape/HeightMapSurround.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\common/Line.h ..\src\ode/odecpp.h ..\src\landscape/PatchSide.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcdialogsHelpDialogscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWButton.h ..\src\common/FileLines.h ..\src\common/Vector.h ..\src\GLW/GLWLabel.h ..\src\GLW/GLWFileView.h ..\src\GLW/GLWWindow.h ..\src\GLW/GLWScrollButton.h ..\src\GLW/GLWidget.h ..\src\dialogs/HelpDialog.h ..\src\dialogs\HelpDialog.cpp ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\common/WindowManager.h ..\src\GLW/GLWTextButton.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\GLW/GLWScrollW.h ..\src\GLW/GLWVisibleWidget.h ..\src\engine/GameStateI.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcserverServerScoreStatescorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\common/OptionsGame.h ..\src\common/ARGParser.h ..\src\weapons/Weapon.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\engine/GameStateStimulusI.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\server/ServerScoreState.h ..\src\tank/TankSort.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsMessageSender.h ..\src\coms/ComsScoreMessage.h ..\src\server\ServerScoreState.cpp ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\server/ServerDialog.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srcclientGLSetupscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLStateExtension.h ..\src\client/GLSetup.h ..\src\client\GLSetup.cpp ..\src\GLEXT/GLState.h
..\src\scorched\release\srccommonTrianglescorched.obj: ..\src\common/Defines.h ..\src\common\Triangle.cpp ..\src\common/Vector.h ..\src\common/Triangle.h ..\src\common/Line.h
..\src\scorched\release\srczlibinfcodesscorched.obj: ..\src\zlib\infcodes.c
..\src\scorched\release\srczlibgzioscorched.obj: ..\src\zlib\gzio.c
..\src\scorched\release\srcdialogsWindDialogscorched.obj: ..\src\GLEXT/GLCamera.h ..\src\client/MainCamera.h ..\src\common/Defines.h ..\src\landscape/Variance.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\landscape/ShadowMap.h ..\src\landscape/PatchGrid.h ..\src\landscape/HeightMap.h ..\src\landscape/GlobalHMap.h ..\src\landscape/Surround.h ..\src\common/Vector.h ..\src\landscape/Wall.h ..\src\landscape/WaterMapPoints.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\GLW/GLWWindow.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWidget.h ..\src\landscape/Smoke.h ..\src\landscape/Landscape.h ..\src\landscape/WaterMap.h ..\src\GLEXT/GLVertexArray.h ..\src\3dsparse/ASEFile.h ..\src\dialogs/WindDialog.h ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\coms/ComsLevelMessage.h ..\src\landscape/SmokeChain.h ..\src\landscape/Sun.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\landscape/TriNode.h ..\src\landscape/TriNodePool.h ..\src\GLEXT/GLFont2d.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\GLEXT/GLMenuI.h ..\src\landscape/Patch.h ..\src\landscape/WaterMapSurround.h ..\src\GLW/GLWVisibleWidget.h ..\src\landscape/SurroundDefs.h ..\src\GLEXT/GLBitmap.h ..\src\GLW/GLWFont.h ..\src\landscape/HeightMapSurround.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\3dsparse/ASEArrayFact.h ..\src\common/Line.h ..\src\dialogs\WindDialog.cpp ..\src\GLEXT/GLVertexTexArray.h ..\src\3dsparse/ASEModel.h ..\src\landscape/PatchSide.h ..\src\common/Face.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcGLWGLWTabscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWScrollPanel.h ..\src\GLW/GLWButton.h ..\src\GLW/GLWLabel.h ..\src\GLW/GLWScrollButton.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLEXT/GLState.h ..\src\GLW/GLWTab.h ..\src\GLW/GLWScrollW.h ..\src\GLW\GLWTab.cpp ..\src\GLW/GLWVisibleWidget.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srccommonVectorLibscorched.obj: ..\src\common/Defines.h ..\src\common/VectorLib.h ..\src\common/Vector.h ..\src\common\VectorLib.cpp
..\src\scorched\release\srcactionsTankFallingscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\actions\TankFalling.cpp ..\src\engine/PhysicsParticleObject.h ..\src\weapons/Weapon.h ..\src\actions/TankMove.h ..\src\engine/ActionBuffer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\tank/TankController.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\actions/TankFalling.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcodefastltsolvescorched.obj: ..\src\ode\fastltsolve.c
..\src\scorched\release\srcXMLXMLFilescorched.obj: ..\src\common/FileLines.h ..\src\XML/XMLFile.h ..\src\XML/expat.h ..\src\XML\XMLFile.cpp ..\src\XML/XMLParser.h
..\src\scorched\release\srcGLWGLWListViewscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLW/GLWButton.h ..\src\common/FileLines.h ..\src\common/Vector.h ..\src\GLW/GLWListView.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWScrollButton.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFont2d.h ..\src\GLW/GLWScrollW.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLW/GLWFont.h ..\src\GLW\GLWListView.cpp
..\src\scorched\release\srcengineMainLoopIscorched.obj: ..\src\common/Vector.h ..\src\engine/MainLoopI.h ..\src\engine\MainLoopI.cpp
..\src\scorched\release\srcweaponsAccessoryscorched.obj: ..\src\weapons\Accessory.cpp ..\src\weapons/Accessory.h
..\src\scorched\release\srclandscapeHeightMapModifierscorched.obj: ..\src\common/Defines.h ..\src\landscape/HeightMap.h ..\src\landscape/HeightMapModifier.h ..\src\common/Vector.h ..\src\common/ProgressCounter.h ..\src\common/RandomGenerator.h ..\src\landscape\HeightMapModifier.cpp
..\src\scorched\release\srczlibdeflatescorched.obj: ..\src\zlib\deflate.c
..\src\scorched\release\srcweaponsAutoDefensescorched.obj: ..\src\weapons/AutoDefense.h ..\src\common/Vector.h ..\src\common/OptionsTransient.h ..\src\weapons\AutoDefense.cpp ..\src\weapons/Accessory.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srclandscapeWaterMapPointsscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\common/Vector.h ..\src\landscape/WaterMapPoints.h ..\src\GLEXT/GLTexture.h ..\src\landscape/WaterMap.h ..\src\GLEXT/GLState.h ..\src\landscape\WaterMapPoints.cpp ..\src\landscape/WaterMapSurround.h ..\src\GLEXT/GLBitmap.h
..\src\scorched\release\srczlibinflatescorched.obj: ..\src\zlib\inflate.c
..\src\scorched\release\srclandscapeWaterMapscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\common/ARGParser.h ..\src\GLEXT/GLCameraFrustum.h ..\src\GLEXT/GLStateExtension.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\common/FileLines.h ..\src\common/Vector.h ..\src\landscape\WaterMap.cpp ..\src\GLEXT/GLTexture.h ..\src\landscape/WaterMap.h ..\src\common/OptionEntry.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\client/OptionsClient.h ..\src\engine/GameState.h ..\src\landscape/WaterMapSurround.h ..\src\GLEXT/GLBitmap.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h
..\src\scorched\release\srczlibzutilscorched.obj: ..\src\zlib\zutil.c
..\src\scorched\release\srcactionsShotProjectileTracerscorched.obj: ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\engine/PhysicsParticleObject.h ..\src\actions/ShotProjectileTracer.h ..\src\sprites/TracerActionRenderer.h ..\src\weapons/Weapon.h ..\src\actions/ShotProjectile.h ..\src\tankgraph/TankMenus.h ..\src\actions\ShotProjectileTracer.cpp ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\engine/ScorchedCollisionIds.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\tankgraph/TracerStore.h ..\src\engine/PhysicsEngine.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\weapons/Accessory.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\tankgraph/TankRenderer.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcspritesExplosionRendererscorched.obj: ..\src\tank/TankShields.h ..\src\common/SoundStore.h ..\src\engine/ActionMeta.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\GLEXT/GLTextureSet.h ..\src\landscape/Variance.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\engine/PhysicsParticleObject.h ..\src\common/ARGParser.h ..\src\GLEXT/GLCameraFrustum.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\weapons/Weapon.h ..\src\landscape/ShadowMap.h ..\src\landscape/PatchGrid.h ..\src\sprites/SmokeActionRenderer.h ..\src\landscape/HeightMap.h ..\src\engine/ActionBuffer.h ..\src\landscape/Surround.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\sprites\ExplosionRenderer.cpp ..\src\tank/TankController.h ..\src\common/Vector.h ..\src\landscape/Wall.h ..\src\landscape/WaterMapPoints.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\sprites/ExplosionTextures.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\GLEXT/GLTexture.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\landscape/Smoke.h ..\src\ode/matrix.h ..\src\landscape/Landscape.h ..\src\tank/TankParachutes.h ..\src\GLEXT/GLVertexArray.h ..\src\landscape/WaterMap.h ..\src\common/OptionEntry.h ..\src\common/Sound.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\actions/SpriteProjectile.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\landscape/SmokeChain.h ..\src\sprites/DebrisActionRenderer.h ..\src\landscape/Sun.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\common/SoundBuffer.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\landscape/TriNode.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\sprites/ExplosionRenderer.h ..\src\landscape/TriNodePool.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\client/OptionsClient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\landscape/Patch.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\landscape/WaterMapSurround.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\landscape/SurroundDefs.h ..\src\tank/TankAutoDefense.h ..\src\GLEXT/GLBitmap.h ..\src\landscape/HeightMapSurround.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\landscape/PatchSide.h ..\src\engine/Action.h
..\src\scorched\release\srctankaiTankAIComputerBuyerscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\tankai\TankAIComputerBuyer.cpp ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\tankai/TankAIComputerBuyer.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srcenginePhysicsParticleObjectscorched.obj: ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\engine/PhysicsParticleObject.h ..\src\engine\PhysicsParticleObject.cpp ..\src\engine/ActionBuffer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcclientClientGameStoppedHandlerscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\client\ClientGameStoppedHandler.cpp ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/ComsGameStoppedMessage.h ..\src\coms/NetMessageHandler.h ..\src\client/ClientGameStoppedHandler.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\coms/ComsMessageHandler.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\client/ClientState.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srclandscapeDeformLandscapescorched.obj: ..\src\common/Defines.h ..\src\landscape\DeformLandscape.cpp ..\src\landscape/HeightMap.h ..\src\landscape/GlobalHMap.h ..\src\common/Vector.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms/ComsLevelMessage.h ..\src\common/ProgressCounter.h ..\src\coms/NetBuffer.h ..\src\landscape/DeformLandscape.h
..\src\scorched\release\srcGLEXTGLConsolescorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLConsoleRules.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\GLEXT/GLConsoleRuleFn.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLConsole.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLConsoleRuleMethod.h ..\src\engine/MainLoopI.h ..\src\GLEXT/GLFont2d.h ..\src\engine/GameState.h ..\src\GLEXT/GLConsoleRule.h ..\src\GLEXT/GLConsoleMethods.h ..\src\common/Keyboard.h ..\src\GLW/GLWFont.h ..\src\engine/GameStateI.h ..\src\GLEXT\GLConsole.cpp ..\src\GLEXT/GLConsoleLines.h
..\src\scorched\release\srcserverServerDialogscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\common/OptionsGame.h ..\src\common/ARGParser.h ..\src\server/OptionsServer.h ..\src\weapons/Weapon.h ..\src\common/FileLines.h ..\src\coms/NetServer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\coms/ComsTextMessage.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\server\ServerDialog.cpp ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\coms/NetBufferUtil.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsMessageSender.h ..\src\server/ServerState.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\server/ServerDialog.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srctankgraphTankMenusscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\weapons/Weapon.h ..\src\GLW/GLWButton.h ..\src\tankgraph/TankMenus.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\tankgraph\TankMenus.cpp ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\GLW/GLWWindow.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\GLEXT/GLTexture.h ..\src\ode/odecpp_old_collision.h ..\src\GLW/GLWidget.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\dialogs/QuitDialog.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLMenu.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\GLW/GLWVisiblePanel.h ..\src\common/WindowManager.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\dialogs/MainMenuDialog.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\GLEXT/GLFont2d.h ..\src\ode/config.h ..\src\GLEXT/GLMenuI.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\GLW/GLWVisibleWidget.h ..\src\client/ClientState.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\GLEXT/GLMenuEntry.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcGLEXTGLFontBannerscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLConsoleRules.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWidget.h ..\src\GLEXT/GLConsoleRuleFn.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLConsole.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFontBanner.h ..\src\GLEXT/GLConsoleRuleMethod.h ..\src\engine/MainLoopI.h ..\src\GLEXT/GLFont2d.h ..\src\engine/GameState.h ..\src\GLEXT/GLConsoleRule.h ..\src\GLEXT/GLConsoleMethods.h ..\src\GLEXT\GLFontBanner.cpp ..\src\GLW/GLWFont.h ..\src\engine/GameStateI.h ..\src\GLEXT/GLConsoleLines.h
..\src\scorched\release\srcclientClientTextHandlerscorched.obj: ..\src\tank/TankShields.h ..\src\engine/ActionMeta.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\weapons/Weapon.h ..\src\GLW/GLWButton.h ..\src\sprites/TalkRenderer.h ..\src\GLW/GLWTextBox.h ..\src\engine/ActionBuffer.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\coms/ComsTextMessage.h ..\src\GLW/GLWLabel.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\GLW/GLWListView.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\GLW/GLWWindow.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWScrollButton.h ..\src\dialogs/ConnectDialog.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\GLW/GLWidget.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\coms/ComsMessageHandler.h ..\src\client\ClientTextHandler.cpp ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLW/GLWTextButton.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFontBanner.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\client/ClientTextHandler.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\GLW/GLWScrollW.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\GLW/GLWVisibleWidget.h ..\src\engine/ActionController.h ..\src\client/MainBanner.h ..\src\client/ClientState.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srccomsNetMessageHandlerscorched.obj: ..\src\coms/NetMessagePool.h ..\src\coms/NetMessage.h ..\src\coms/NetMessageHandler.h ..\src\coms\NetMessageHandler.cpp ..\src\coms/NetBuffer.h
..\src\scorched\release\srctankaiTankAIComputerscorched.obj: ..\src\engine/ActionMeta.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\common/OptionsGame.h ..\src\tank/TankBatteries.h ..\src\common/ARGParser.h ..\src\tankai/TankAIStrings.h ..\src\weapons/Weapon.h ..\src\actions/TankSay.h ..\src\tankai/TankAIComputer.h ..\src\engine/ActionBuffer.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tankai\TankAIComputer.cpp ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\coms/ComsPlayedMoveMessage.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\coms/ComsDefenseMessage.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsMessageSender.h ..\src\server/ServerShotHolder.h ..\src\engine/ActionController.h ..\src\tank/TankAutoDefense.h ..\src\tankai/TankAIComputerBuyer.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcweaponsWeaponProjectilescorched.obj: ..\src\tank/TankShields.h ..\src\engine/ActionMeta.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\engine/PhysicsParticleObject.h ..\src\weapons/Weapon.h ..\src\actions/ShotProjectile.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\actions/ShotProjectileExplosion.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\weapons\WeaponProjectile.cpp ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\weapons/WeaponProjectile.h ..\src\engine/Action.h
..\src\scorched\release\srclandscapeVariancescorched.obj: ..\src\common/Defines.h ..\src\landscape/Variance.h ..\src\landscape/HeightMap.h ..\src\common/Vector.h ..\src\common/ProgressCounter.h ..\src\landscape\Variance.cpp
..\src\scorched\release\srcengineActionBufferscorched.obj: ..\src\engine/ActionMeta.h ..\src\engine/ActionBuffer.h ..\src\coms/NetBuffer.h ..\src\engine/Action.h ..\src\engine\ActionBuffer.cpp
..\src\scorched\release\srcclientClientStartGameHandlerscorched.obj: ..\src\common/SoundStore.h ..\src\engine/ActionMeta.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\common/OptionsGame.h ..\src\tank/TankBatteries.h ..\src\common/ARGParser.h ..\src\weapons/Weapon.h ..\src\engine/ActionBuffer.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\tank/TankStart.h ..\src\client/ClientStartGameHandler.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\coms/ComsMessageHandler.h ..\src\common/Sound.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\common/SoundBuffer.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\coms/ComsStartGameMessage.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\client\ClientStartGameHandler.cpp ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\client/ClientState.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\coms/ComsPlayerStateMessage.h ..\src\ode/odecpp.h ..\src\client/ClientNewGameHandler.h ..\src\engine/Action.h
..\src\scorched\release\srcdialogsPlayerDialogscorched.obj: ..\src\common/Defines.h ..\src\common/ARGParser.h ..\src\GLW/GLWButton.h ..\src\GLW/GLWTextBox.h ..\src\common/FileLines.h ..\src\GLW/GLWLabel.h ..\src\common/Vector.h ..\src\dialogs/PlayerDialog.h ..\src\GLW/GLWWindow.h ..\src\GLW/GLWScrollButton.h ..\src\dialogs\PlayerDialog.cpp ..\src\GLW/GLWidget.h ..\src\tankgraph/GLWTankViewer.h ..\src\GLEXT/GLVertexArray.h ..\src\common/OptionEntry.h ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLW/GLWTextButton.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\client/OptionsClient.h ..\src\engine/GameState.h ..\src\GLW/GLWScrollW.h ..\src\GLW/GLWVisibleWidget.h ..\src\client/ClientState.h ..\src\coms/NetBuffer.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srccomsComsGameStoppedMessagescorched.obj: ..\src\coms\ComsGameStoppedMessage.cpp ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms/ComsGameStoppedMessage.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srccomsNetBufferUtilscorched.obj: ..\src\coms/NetMessagePool.h ..\src\coms/NetMessage.h ..\src\coms/NetBufferUtil.h ..\src\coms\NetBufferUtil.cpp ..\src\coms/NetBuffer.h
..\src\scorched\release\srcGLEXTGLTexturescorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLStateExtension.h ..\src\GLEXT\GLTexture.cpp ..\src\GLEXT/GLTexture.h ..\src\GLEXT/GLState.h
..\src\scorched\release\srclandscapePatchSidescorched.obj: ..\src\common/Defines.h ..\src\landscape/Variance.h ..\src\landscape/HeightMap.h ..\src\landscape\PatchSide.cpp ..\src\common/Vector.h ..\src\GLEXT/GLVar.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\landscape/TriNode.h ..\src\landscape/TriNodePool.h ..\src\landscape/PatchSide.h ..\src\landscape/PatchVar.h
..\src\scorched\release\srclandscapeHemispherescorched.obj: ..\src\common/Defines.h ..\src\landscape/Hemisphere.h ..\src\common/Vector.h ..\src\GLEXT/GLState.h ..\src\landscape\Hemisphere.cpp
..\src\scorched\release\srccommonSoundBufferscorched.obj: ..\src\common\SoundBuffer.cpp ..\src\common/Sound.h ..\src\common/SoundBuffer.h
..\src\scorched\release\srctankTankParachutesscorched.obj: ..\src\tank/TankParachutes.h ..\src\tank\TankParachutes.cpp ..\src\coms/NetBuffer.h
..\src\scorched\release\srcengineGameStateStimulusIscorched.obj: ..\src\engine/GameStateStimulusI.h ..\src\engine\GameStateStimulusI.cpp
..\src\scorched\release\srcdialogsQuitDialogscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWButton.h ..\src\common/Vector.h ..\src\GLW/GLWLabel.h ..\src\GLW/GLWWindow.h ..\src\GLW/GLWidget.h ..\src\dialogs\QuitDialog.cpp ..\src\dialogs/QuitDialog.h ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\common/WindowManager.h ..\src\GLW/GLWTextButton.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\engine/MainLoop.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\common/Timer.h ..\src\GLW/GLWVisibleWidget.h ..\src\engine/GameStateI.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcactionsExplosionscorched.obj: ..\src\engine/ActionMeta.h ..\src\common/SoundStore.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\GLEXT/GLTextureSet.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\common/OptionsGame.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\engine/PhysicsParticleObject.h ..\src\common/ARGParser.h ..\src\weapons/Weapon.h ..\src\actions/ShotProjectile.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\tank/TankController.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\sprites/ExplosionTextures.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\GLEXT/GLTexture.h ..\src\actions\Explosion.cpp ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\common/Sound.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\common/SoundBuffer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\sprites/ExplosionRenderer.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\actions/Explosion.h ..\src\landscape/DeformTextures.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\landscape/DeformLandscape.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srctankaiTankAIComputerMoronscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\tankai/TankAIComputer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tankai\TankAIComputerMoron.cpp ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\tankai/TankAIComputerBuyer.h ..\src\coms/NetBuffer.h ..\src\tankai/TankAIComputerMoron.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srctankTankPhysicsscorched.obj: ..\src\engine/ActionMeta.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\engine/ActionBuffer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\engine/MainLoopI.h ..\src\tank/TankLib.h ..\src\ode/timer.h ..\src\tank\TankPhysics.cpp ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\tank/TankScore.h ..\src\tank/TankPhysics.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcactionsWallHitscorched.obj: ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\actions/WallHit.h ..\src\GLEXT/GLImage.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\sprites/WallActionRenderer.h ..\src\actions\WallHit.cpp ..\src\GLEXT/GLState.h ..\src\common/OptionsTransient.h ..\src\coms/NetBuffer.h ..\src\engine/Action.h
..\src\scorched\release\srcweaponsShieldReflectivescorched.obj: ..\src\weapons\ShieldReflective.cpp ..\src\common/Vector.h ..\src\weapons/Accessory.h ..\src\weapons/Shield.h ..\src\weapons/ShieldReflective.h
..\src\scorched\release\srcGLWGLWSpinBoxscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLW/GLWButton.h ..\src\GLW\GLWSpinBox.cpp ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWidget.h ..\src\GLW/GLWSpinBox.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFont2d.h ..\src\common/Keyboard.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLW/GLWFont.h
..\src\scorched\release\srcengineScorchedCollisionHandlerscorched.obj: ..\src\engine/ActionMeta.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\engine\ScorchedCollisionHandler.cpp ..\src\ode/memory.h ..\src\ode/geom.h ..\src\actions/WallHit.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\engine/PhysicsParticleObject.h ..\src\weapons/Weapon.h ..\src\actions/ShieldHit.h ..\src\engine/ActionBuffer.h ..\src\actions/ShotProjectile.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\engine/ScorchedCollisionHandler.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcGLWGLWFontscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLW\GLWFont.cpp ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFont2d.h ..\src\GLW/GLWFont.h
..\src\scorched\release\srcserverServerTooFewPlayersStimulusscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\common/OptionsGame.h ..\src\tank/TankBatteries.h ..\src\common/ARGParser.h ..\src\weapons/Weapon.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\server\ServerTooFewPlayersStimulus.cpp ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/ComsGameStoppedMessage.h ..\src\engine/GameStateStimulusI.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsMessageSender.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\server/ServerDialog.h ..\src\server/ServerTooFewPlayersStimulus.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srctankTankWeaponscorched.obj: ..\src\common/Defines.h ..\src\weapons/Weapon.h ..\src\tank/TankWeapon.h ..\src\tank\TankWeapon.cpp ..\src\weapons/Accessory.h ..\src\coms/NetBuffer.h ..\src\weapons/AccessoryStore.h
..\src\scorched\release\srcGLWGLWidgetscorched.obj: ..\src\common/Defines.h ..\src\GLW\GLWidget.cpp ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcscorchedDisplayDialogscorched.obj: ..\src\common/ARGParser.h ..\src\common/FileLines.h ..\src\scorched/MainDialog.h ..\src\scorched\DisplayDialog.cpp ..\src\common/OptionEntry.h ..\src\scorched/DisplayDialog.h ..\src\client/OptionsClient.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srclandscapeSurroundscorched.obj: ..\src\GLEXT/GLCamera.h ..\src\client/MainCamera.h ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\common/ARGParser.h ..\src\landscape/Hemisphere.h ..\src\landscape/HeightMap.h ..\src\landscape/Surround.h ..\src\common/FileLines.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\common/OptionEntry.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\common/OptionsTransient.h ..\src\client/OptionsClient.h ..\src\engine/GameState.h ..\src\GLEXT/GLMenuI.h ..\src\common/FileList.h ..\src\landscape/SurroundDefs.h ..\src\landscape\Surround.cpp ..\src\GLEXT/GLBitmap.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\common/Line.h
..\src\scorched\release\srcenginePhysicsParticlescorched.obj: ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\engine/PhysicsParticleObject.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\engine\PhysicsParticle.cpp ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\ode/config.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcGLWGLWPanelscorched.obj: ..\src\common/Defines.h ..\src\GLW\GLWPanel.cpp ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srclandscapeHeightMapCollisionscorched.obj: ..\src\engine/ActionMeta.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\landscape/HeightMap.h ..\src\engine/ActionBuffer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\common/Triangle.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\landscape/HeightMapCollision.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\engine/ScorchedCollisionIds.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\landscape\HeightMapCollision.cpp ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\common/Line.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srctankTankModelIdscorched.obj: ..\src\tank\TankModelId.cpp ..\src\tank/TankModelId.h
..\src\scorched\release\srcclientMainCamerascorched.obj: ..\src\common/SoundStore.h ..\src\tank/TankShields.h ..\src\GLEXT/GLCamera.h ..\src\client/MainCamera.h ..\src\engine/ActionMeta.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\engine/PhysicsParticleObject.h ..\src\client\MainCamera.cpp ..\src\weapons/Weapon.h ..\src\landscape/HeightMap.h ..\src\landscape/GlobalHMap.h ..\src\actions/ShotProjectile.h ..\src\engine/ActionBuffer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\GLEXT/GLConsoleRules.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\GLW/GLWWindow.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\GLEXT/GLTexture.h ..\src\ode/odecpp_old_collision.h ..\src\GLW/GLWidget.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\GLEXT/GLConsoleRuleFn.h ..\src\common/Sound.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLMenu.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\GLW/GLWVisiblePanel.h ..\src\coms/ComsLevelMessage.h ..\src\GLEXT/GLConsole.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\dialogs/MainMenuDialog.h ..\src\common/SoundBuffer.h ..\src\GLEXT/GLConsoleRuleMethod.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\GLEXT/GLFont2d.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\GLEXT/GLConsoleRule.h ..\src\common/Keyboard.h ..\src\GLEXT/GLConsoleMethods.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\GLW/GLWVisibleWidget.h ..\src\engine/ActionController.h ..\src\tank/TankAutoDefense.h ..\src\GLEXT/GLBitmap.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\weapons/Shield.h ..\src\GLEXT/GLMenuEntry.h ..\src\ode/odemath.h ..\src\common/Line.h ..\src\ode/odecpp.h ..\src\GLEXT/GLConsoleLines.h ..\src\GLW/GLWPanel.h ..\src\engine/Action.h
..\src\scorched\release\srcdialogsKillDialogscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\dialogs\KillDialog.cpp ..\src\GLW/GLWButton.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\GLW/GLWLabel.h ..\src\dialogs/KillDialog.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\GLW/GLWWindow.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\GLW/GLWidget.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\GLW/GLWVisiblePanel.h ..\src\common/WindowManager.h ..\src\GLW/GLWTextButton.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\engine/MainLoop.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\common/Timer.h ..\src\ode/odecpp_collision.h ..\src\GLW/GLWVisibleWidget.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srclandscapeGlobalHMapscorched.obj: ..\src\common/Defines.h ..\src\common/OptionsGame.h ..\src\common/ARGParser.h ..\src\landscape/HeightMap.h ..\src\landscape/GlobalHMap.h ..\src\common/FileLines.h ..\src\landscape/HeightMapModifier.h ..\src\common/Vector.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\landscape\GlobalHMap.cpp ..\src\common/OptionEntry.h ..\src\coms/ComsLevelMessage.h ..\src\common/ProgressCounter.h ..\src\zlib/zlib.h ..\src\common/RandomGenerator.h ..\src\coms/NetBuffer.h
..\src\scorched\release\src3dsparseMeshLODscorched.obj: ..\src\3dsparse/MeshLOD.h ..\src\3dsparse/MeshLODTri.h ..\src\common/Vector.h ..\src\3dsparse\MeshLOD.cpp ..\src\3dsparse/MeshLODVector.h ..\src\common/Face.h
..\src\scorched\release\srccommonMousescorched.obj: ..\src\common/Mouse.h ..\src\common/Vector.h ..\src\engine/MainLoopI.h ..\src\engine/GameState.h ..\src\common\Mouse.cpp
..\src\scorched\release\srcactionsShotProjectilescorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\sprites/FlareActionRenderer.h ..\src\engine/PhysicsParticleObject.h ..\src\weapons/Weapon.h ..\src\actions/ShotProjectile.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\actions\ShotProjectile.cpp ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcclientShotTimerscorched.obj: ..\src\common/Defines.h ..\src\common/OptionsGame.h ..\src\GLEXT/GLImage.h ..\src\common/ARGParser.h ..\src\client\ShotTimer.cpp ..\src\common/FileLines.h ..\src\common/Vector.h ..\src\engine/GameStateStimulusI.h ..\src\GLEXT/GLTexture.h ..\src\common/OptionEntry.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\GLEXT/GLFont2d.h ..\src\engine/GameState.h ..\src\GLW/GLWFont.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\client/ShotTimer.h
..\src\scorched\release\srcGLEXTGLCameraFrustumscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLCameraFrustum.h ..\src\common/Vector.h ..\src\common/KeyboardHistory.h ..\src\GLEXT\GLCameraFrustum.cpp ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\engine/GameState.h ..\src\engine/GameStateI.h
..\src\scorched\release\srcGLWGLWScrollHscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWScrollH.h ..\src\GLW\GLWScrollH.cpp ..\src\GLW/GLWButton.h ..\src\GLW/GLWScrollButton.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLW/GLWVisibleWidget.h
..\src\scorched\release\srccomsComsAddPlayerMessagescorched.obj: ..\src\coms/ComsAddPlayerMessage.h ..\src\common/Vector.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms\ComsAddPlayerMessage.cpp ..\src\coms/NetBuffer.h
..\src\scorched\release\srcGLEXTGLMenuEntryscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLEXT\GLMenuEntry.cpp ..\src\GLEXT/GLFont2d.h ..\src\GLEXT/GLMenuI.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLEXT/GLMenuEntry.h
..\src\scorched\release\srcweaponsAccessoryStorescorched.obj: ..\src\common/Defines.h ..\src\weapons/WeaponTracer.h ..\src\weapons/Weapon.h ..\src\weapons/WeaponRiotBomb.h ..\src\weapons\AccessoryStore.cpp ..\src\weapons/WeaponFunky.h ..\src\weapons/AutoDefense.h ..\src\common/Vector.h ..\src\weapons/WeaponMirv.h ..\src\weapons/Parachute.h ..\src\weapons/WeaponClod.h ..\src\weapons/Accessory.h ..\src\coms/NetBuffer.h ..\src\weapons/Battery.h ..\src\weapons/AccessoryStore.h ..\src\weapons/Shield.h ..\src\weapons/WeaponProjectile.h ..\src\weapons/ShieldReflectiveMag.h ..\src\weapons/ShieldReflective.h
..\src\scorched\release\srccommonOptionsGamescorched.obj: ..\src\common/OptionsGame.h ..\src\common/ARGParser.h ..\src\common/FileLines.h ..\src\common\OptionsGame.cpp ..\src\common/Vector.h ..\src\common/OptionEntry.h ..\src\common/OptionsTransient.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srctankaiTankAIComputerRandomscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\tankai/TankAIComputer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\tankai/TankAIComputerRandom.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tankai/TankAIStore.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tankai\TankAIComputerRandom.cpp ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\tankai/TankAIComputerBuyer.h ..\src\coms/NetBuffer.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srcactionsTankMovescorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\actions/TankMove.h ..\src\actions\TankMove.cpp ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcweaponsWeaponFunkyscorched.obj: ..\src\engine/ActionMeta.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\engine/PhysicsParticleObject.h ..\src\weapons/Weapon.h ..\src\actions/ShotProjectile.h ..\src\ode/error.h ..\src\actions/ShotProjectileExplosion.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\weapons/WeaponFunky.h ..\src\weapons\WeaponFunky.cpp ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\actions/ShotProjectileFunky.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srccommonFileListscorched.obj: ..\src\common\FileList.cpp ..\src\common/FileList.h
..\src\scorched\release\srcclientClientShotStatescorched.obj: ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\engine/ActionBuffer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\client/ClientShotState.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\engine/GameStateStimulusI.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\client\ClientShotState.cpp ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srctankaiTankAIHumanscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\common/SoundStore.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\tankai\TankAIHuman.cpp ..\src\common/ARGParser.h ..\src\weapons/Weapon.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\common/Sound.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\common/SoundBuffer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\client/OptionsClient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\common/Keyboard.h ..\src\weapons/Accessory.h ..\src\tank/TankAI.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\tankai/TankAIHuman.h ..\src\ode/odecpp.h
..\src\scorched\release\srccommonOptionEntryscorched.obj: ..\src\common\OptionEntry.cpp ..\src\common/ARGParser.h ..\src\common/FileLines.h ..\src\XML/XMLFile.h ..\src\common/OptionEntry.h ..\src\XML/expat.h ..\src\XML/XMLParser.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srcclientClientNewGameHandlerscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\GLEXT/GLTextureSet.h ..\src\ode/geom.h ..\src\landscape/Variance.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\client/SpeedChange.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\weapons/Weapon.h ..\src\GLW/GLWButton.h ..\src\landscape/ShadowMap.h ..\src\landscape/PatchGrid.h ..\src\landscape/HeightMap.h ..\src\GLW/GLWTextBox.h ..\src\landscape/GlobalHMap.h ..\src\landscape/Surround.h ..\src\tankgraph/TankMenus.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\tank/TankStart.h ..\src\GLW/GLWLabel.h ..\src\common/Vector.h ..\src\landscape/Wall.h ..\src\landscape/WaterMapPoints.h ..\src\ode/space.h ..\src\GLW/GLWListView.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\sprites/ExplosionTextures.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\GLW/GLWWindow.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWScrollButton.h ..\src\dialogs/ConnectDialog.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\GLW/GLWidget.h ..\src\landscape/Smoke.h ..\src\ode/matrix.h ..\src\landscape/Landscape.h ..\src\tank/TankParachutes.h ..\src\landscape/WaterMap.h ..\src\coms/ComsMessageHandler.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\GLW/GLWVisiblePanel.h ..\src\coms/ComsLevelMessage.h ..\src\landscape/SmokeChain.h ..\src\GLW/GLWTextButton.h ..\src\GLW/GLWProgress.h ..\src\landscape/Sun.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\GLEXT/GLFontBanner.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\landscape/TriNode.h ..\src\tankgraph/TracerStore.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\engine/MainLoop.h ..\src\landscape/TriNodePool.h ..\src\dialogs/ProgressDialog.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\GLEXT/GLMenuI.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\landscape/Patch.h ..\src\GLW/GLWScrollW.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\coms/ComsNewGameMessage.h ..\src\landscape/WaterMapSurround.h ..\src\tank/TankState.h ..\src\common/Timer.h ..\src\ode/odecpp_collision.h ..\src\GLW/GLWVisibleWidget.h ..\src\common/RandomGenerator.h ..\src\landscape/SurroundDefs.h ..\src\client/MainBanner.h ..\src\client/ClientState.h ..\src\client\ClientNewGameHandler.cpp ..\src\tank/TankAutoDefense.h ..\src\GLEXT/GLBitmap.h ..\src\landscape/HeightMapSurround.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\tankgraph/TankRenderer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\GLEXT/GLLenseFlare.h ..\src\ode/odecpp.h ..\src\client/ClientNewGameHandler.h ..\src\landscape/PatchSide.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srczlibinftreesscorched.obj: ..\src\zlib\inftrees.c
..\src\scorched\release\srccomsComsNewGameMessagescorched.obj: ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms/ComsLevelMessage.h ..\src\coms\ComsNewGameMessage.cpp ..\src\coms/ComsNewGameMessage.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srclandscapeWallscorched.obj: ..\src\common/SoundStore.h ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\common/Vector.h ..\src\landscape/Wall.h ..\src\GLEXT/GLTexture.h ..\src\common/Sound.h ..\src\GLEXT/GLState.h ..\src\common/SoundBuffer.h ..\src\common/OptionsTransient.h ..\src\landscape\Wall.cpp ..\src\GLEXT/GLBitmap.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srcGLWGLWVisibleWidgetscorched.obj: ..\src\common/Defines.h ..\src\common/Vector.h ..\src\GLW\GLWVisibleWidget.cpp ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLW/GLWVisibleWidget.h
..\src\scorched\release\src3dsparseASEStorescorched.obj: ..\src\common/Defines.h ..\src\3dsparse/ASEStore.h ..\src\3dsparse\ASEStore.cpp ..\src\common/Vector.h ..\src\GLEXT/GLVertexArray.h ..\src\3dsparse/ASEFile.h ..\src\GLEXT/GLState.h ..\src\3dsparse/ASEArrayFact.h ..\src\GLEXT/GLVertexTexArray.h ..\src\3dsparse/ASEModel.h ..\src\common/Face.h
..\src\scorched\release\srcclientSpeedChangescorched.obj: ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\GLEXT/GLImage.h ..\src\client/SpeedChange.h ..\src\engine/ActionBuffer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\GLEXT/GLTexture.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\client\SpeedChange.cpp ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\GLEXT/GLFont2d.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\common/Keyboard.h ..\src\ode/odecpp_collision.h ..\src\engine/ActionController.h ..\src\GLW/GLWFont.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcserverServerBuyAccessoryHandlerscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\common/OptionsGame.h ..\src\tank/TankBatteries.h ..\src\common/ARGParser.h ..\src\weapons/Weapon.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\coms/ComsMessageHandler.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\coms/ComsPlayedMoveMessage.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsMessageSender.h ..\src\server/ServerState.h ..\src\coms/ComsBuyAccessoryMessage.h ..\src\server/ServerShotHolder.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\server/ServerBuyAccessoryHandler.h ..\src\server\ServerBuyAccessoryHandler.cpp
..\src\scorched\release\srcspritesFlareActionRendererscorched.obj: ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\landscape/Variance.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\GLEXT/GLImage.h ..\src\sprites/FlareActionRenderer.h ..\src\common/ARGParser.h ..\src\engine/PhysicsParticleObject.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\landscape/ShadowMap.h ..\src\landscape/PatchGrid.h ..\src\landscape/HeightMap.h ..\src\landscape/GlobalHMap.h ..\src\landscape/Surround.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\landscape/Wall.h ..\src\landscape/WaterMapPoints.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\GLEXT/GLTexture.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\landscape/Smoke.h ..\src\ode/matrix.h ..\src\landscape/Landscape.h ..\src\landscape/WaterMap.h ..\src\common/OptionEntry.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\coms/ComsLevelMessage.h ..\src\landscape/SmokeChain.h ..\src\landscape/Sun.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\landscape/TriNode.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\landscape/TriNodePool.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\client/OptionsClient.h ..\src\engine/GameState.h ..\src\landscape/Patch.h ..\src\landscape/WaterMapSurround.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\landscape/SurroundDefs.h ..\src\GLEXT/GLBitmap.h ..\src\landscape/HeightMapSurround.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\GLEXT/GLLenseFlare.h ..\src\ode/odecpp.h ..\src\sprites\FlareActionRenderer.cpp ..\src\landscape/PatchSide.h ..\src\engine/Action.h
..\src\scorched\release\srcscorchedNetDialogscorched.obj: ..\src\common/Defines.h ..\src\common/ARGParser.h ..\src\scorched\NetDialog.cpp ..\src\common/FileLines.h ..\src\scorched/MainDialog.h ..\src\common/OptionEntry.h ..\src\client/OptionsClient.h ..\src\scorched/NetDialog.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srcodemiscscorched.obj: ..\src\ode/common.h ..\src\ode/config.h ..\src\ode\misc.cpp ..\src\ode/matrix.h ..\src\ode/misc.h ..\src\ode/error.h
..\src\scorched\release\srcweaponsShieldscorched.obj: ..\src\common/Defines.h ..\src\common/VectorLib.h ..\src\common/Vector.h ..\src\weapons\Shield.cpp ..\src\weapons/Accessory.h ..\src\weapons/Shield.h
..\src\scorched\release\srcdialogsMainMenuDialogscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\dialogs\MainMenuDialog.cpp ..\src\common/Vector.h ..\src\GLW/GLWWindow.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLMenu.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLEXT/GLState.h ..\src\dialogs/MainMenuDialog.h ..\src\GLEXT/GLFont2d.h ..\src\GLEXT/GLMenuI.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLEXT/GLMenuEntry.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcdialogsBuyAccessoryDialogscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\common/OptionsGame.h ..\src\tank/TankBatteries.h ..\src\dialogs/BuyAccessoryDialog.h ..\src\common/ARGParser.h ..\src\GLW/GLWScrollPanel.h ..\src\weapons/Weapon.h ..\src\GLW/GLWButton.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\GLW/GLWFlag.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\GLW/GLWLabel.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\GLW/GLWWindow.h ..\src\ode/collision_space.h ..\src\GLW/GLWScrollButton.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\GLW/GLWidget.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\dialogs\BuyAccessoryDialog.cpp ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\GLW/GLWVisiblePanel.h ..\src\common/WindowManager.h ..\src\GLW/GLWTextButton.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\GLW/GLWTab.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\GLW/GLWScrollW.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsMessageSender.h ..\src\coms/ComsBuyAccessoryMessage.h ..\src\GLW/GLWVisibleWidget.h ..\src\client/ClientState.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\weapons/AccessoryStore.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcodejointscorched.obj: ..\src\ode/common.h ..\src\ode/rotation.h ..\src\ode/odemath.h ..\src\ode/config.h ..\src\ode/matrix.h ..\src\ode\joint.cpp ..\src\ode/error.h
..\src\scorched\release\srczlibcrc32scorched.obj: ..\src\zlib\crc32.c
..\src\scorched\release\srcactionsShotProjectileRiotscorched.obj: ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\engine/PhysicsParticleObject.h ..\src\weapons/Weapon.h ..\src\engine/ActionBuffer.h ..\src\actions/ShotProjectile.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\engine/ScorchedCollisionIds.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\ode/config.h ..\src\actions\ShotProjectileRiot.cpp ..\src\engine/GameState.h ..\src\weapons/Accessory.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\actions/ShotProjectileRiot.h ..\src\engine/ActionController.h ..\src\coms/NetBuffer.h ..\src\actions/Explosion.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcGLEXTGLConsoleLinesscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFont2d.h ..\src\GLEXT\GLConsoleLines.cpp ..\src\GLEXT/GLConsoleLines.h
..\src\scorched\release\srctankgraphTankModelRendererscorched.obj: ..\src\GLEXT/GLCamera.h ..\src\client/MainCamera.h ..\src\engine/ActionMeta.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\landscape/Variance.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\common/ARGParser.h ..\src\engine/PhysicsParticleObject.h ..\src\landscape/Hemisphere.h ..\src\GLEXT/GLCameraFrustum.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\weapons/Weapon.h ..\src\landscape/ShadowMap.h ..\src\landscape/PatchGrid.h ..\src\landscape/HeightMap.h ..\src\landscape/Surround.h ..\src\landscape/GlobalHMap.h ..\src\common/FileLines.h ..\src\tankgraph\TankModelRenderer.cpp ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\landscape/Wall.h ..\src\landscape/WaterMapPoints.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\actions/TankFalling.h ..\src\tankgraph/TankModelStore.h ..\src\GLEXT/GLTexture.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\landscape/Smoke.h ..\src\ode/matrix.h ..\src\landscape/Landscape.h ..\src\tank/TankParachutes.h ..\src\landscape/WaterMap.h ..\src\GLEXT/GLVertexArray.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\landscape/SmokeChain.h ..\src\coms/ComsLevelMessage.h ..\src\landscape/Sun.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\landscape/TriNode.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\GLEXT/GLFont2d.h ..\src\landscape/TriNodePool.h ..\src\ode/config.h ..\src\client/OptionsClient.h ..\src\common/OptionsTransient.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\landscape/Patch.h ..\src\tankgraph/TankMesh.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\landscape/WaterMapSurround.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\landscape/SurroundDefs.h ..\src\tankgraph/TankModelRenderer.h ..\src\GLW/GLWFont.h ..\src\GLEXT/GLBitmap.h ..\src\tank/TankAutoDefense.h ..\src\landscape/HeightMapSurround.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\tankgraph/TankModel.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\common/Line.h ..\src\ode/odecpp.h ..\src\landscape/PatchSide.h ..\src\engine/Action.h
..\src\scorched\release\srcdialogsTankDialogscorched.obj: ..\src\tank/TankShields.h ..\src\GLEXT/GLCamera.h ..\src\client/MainCamera.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\GLW/GLWWindow.h ..\src\ode/collision_space.h ..\src\GLEXT/GLTexture.h ..\src\ode/odecpp_old_collision.h ..\src\dialogs\TankDialog.cpp ..\src\ode/mass.h ..\src\GLW/GLWidget.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\GLEXT/GLVertexArray.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\dialogs/TankDialog.h ..\src\GLW/GLWVisiblePanel.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\GLEXT/GLFont2d.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\GLEXT/GLMenuI.h ..\src\tankgraph/TankMesh.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\GLW/GLWVisibleWidget.h ..\src\tankgraph/TankModelRenderer.h ..\src\GLW/GLWFont.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\tankgraph/TankModel.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\common/Line.h ..\src\ode/odecpp.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcodeodescorched.obj: ..\src\ode/common.h ..\src\ode/ode.h ..\src\ode/memory.h ..\src\ode/odecpp_collision.h ..\src\ode/geom.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\ode/rotation.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/timer.h ..\src\ode\ode.cpp ..\src\ode/odecpp_old_collision.h ..\src\ode/odemath.h ..\src\ode/config.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\ode/odecpp.h ..\src\ode/error.h
..\src\scorched\release\srcodememoryscorched.obj: ..\src\ode/memory.h ..\src\ode/config.h ..\src\ode\memory.cpp ..\src\ode/error.h
..\src\scorched\release\srcclientClientStatescorched.obj: ..\src\tank/TankShields.h ..\src\GLEXT/GLCamera.h ..\src\client/MainCamera.h ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\landscape/Variance.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\GLEXT/GLImage.h ..\src\client/SpeedChange.h ..\src\GLEXT/GLCameraFrustum.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\weapons/Weapon.h ..\src\landscape/ShadowMap.h ..\src\landscape/PatchGrid.h ..\src\landscape/HeightMap.h ..\src\landscape/Surround.h ..\src\client/Main2DCamera.h ..\src\engine/ActionBuffer.h ..\src\tankgraph/TankMenus.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\client/ClientShotState.h ..\src\client\ClientState.cpp ..\src\GLEXT/GLConsoleRules.h ..\src\client/ClientNextRoundState.h ..\src\common/Vector.h ..\src\landscape/Wall.h ..\src\landscape/WaterMapPoints.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\GLW/GLWWindow.h ..\src\ode/misc.h ..\src\engine/GameStateStimulusI.h ..\src\ode/collision_space.h ..\src\GLEXT/GLTexture.h ..\src\ode/odecpp_old_collision.h ..\src\GLW/GLWidget.h ..\src\ode/mass.h ..\src\landscape/Smoke.h ..\src\ode/matrix.h ..\src\landscape/Landscape.h ..\src\tank/TankParachutes.h ..\src\landscape/WaterMap.h ..\src\GLEXT/GLConsoleRuleFn.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLEXT/GLConsole.h ..\src\landscape/SmokeChain.h ..\src\common/WindowManager.h ..\src\landscape/Sun.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\GLEXT/GLFontBanner.h ..\src\GLEXT/GLConsoleRuleMethod.h ..\src\engine/MainLoopI.h ..\src\landscape/TriNode.h ..\src\ode/timer.h ..\src\tankgraph/TracerStore.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\engine/MainLoop.h ..\src\GLEXT/GLFont2d.h ..\src\landscape/TriNodePool.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\tankai/TankAIHumanCtrl.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\landscape/Patch.h ..\src\GLEXT/GLConsoleRule.h ..\src\GLEXT/GLConsoleMethods.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\landscape/WaterMapSurround.h ..\src\tank/TankState.h ..\src\common/Timer.h ..\src\ode/odecpp_collision.h ..\src\GLW/GLWVisibleWidget.h ..\src\engine/ActionController.h ..\src\landscape/SurroundDefs.h ..\src\engine/FrameTimer.h ..\src\client/ClientState.h ..\src\client/MainBanner.h ..\src\GLEXT/GLBitmap.h ..\src\tank/TankAutoDefense.h ..\src\landscape/HeightMapSurround.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\tankgraph/TankRenderer.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\common/Line.h ..\src\client/ShotTimer.h ..\src\ode/odecpp.h ..\src\GLEXT/GLConsoleLines.h ..\src\landscape/PatchSide.h ..\src\GLW/GLWPanel.h ..\src\engine/Action.h
..\src\scorched\release\srccomsNetMessagePoolscorched.obj: ..\src\coms/NetMessagePool.h ..\src\coms/NetMessage.h ..\src\coms\NetMessagePool.cpp ..\src\coms/NetBuffer.h
..\src\scorched\release\srcGLWGLWTextButtonscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWButton.h ..\src\GLW/GLWLabel.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLW/GLWTextButton.h ..\src\GLEXT/GLState.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLW\GLWTextButton.cpp
..\src\scorched\release\srcXMLxmlrolescorched.obj: ..\src\XML\xmlrole.c ..\src\XML/expat_config.h
..\src\scorched\release\srcserverServerNewGameStatescorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\common/OptionsGame.h ..\src\common/ARGParser.h ..\src\server/OptionsServer.h ..\src\weapons/Weapon.h ..\src\landscape/HeightMap.h ..\src\landscape/GlobalHMap.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\tank/TankStart.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\server\ServerNewGameState.cpp ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\coms/ComsLevelMessage.h ..\src\tank/TankContainer.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\coms/ComsNewGameMessage.h ..\src\tank/TankState.h ..\src\common/Timer.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsMessageSender.h ..\src\server/ServerState.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\server/ServerDialog.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\server/ServerNewGameState.h ..\src\ode/odecpp.h
..\src\scorched\release\srcweaponsWeaponTracerscorched.obj: ..\src\engine/ActionMeta.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\engine/PhysicsParticleObject.h ..\src\actions/ShotProjectileTracer.h ..\src\weapons/WeaponTracer.h ..\src\weapons/Weapon.h ..\src\actions/ShotProjectile.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\weapons\WeaponTracer.cpp ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcodematrixscorched.obj: ..\src\ode/common.h ..\src\ode/config.h ..\src\ode/matrix.h ..\src\ode\matrix.cpp ..\src\ode/error.h
..\src\scorched\release\srcactionsTankResignscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\actions\TankResign.cpp ..\src\ode/space.h ..\src\actions/TankResign.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\GLW/GLWidget.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFontBanner.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\client/MainBanner.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcGLEXTGLVertexTexArrayscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLStateExtension.h ..\src\GLEXT\GLVertexTexArray.cpp ..\src\GLEXT/GLVertexArray.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLVertexTexArray.h
..\src\scorched\release\srctankTankAutoDefensescorched.obj: ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\tank\TankAutoDefense.cpp
..\src\scorched\release\srctankaiTankAIStringsscorched.obj: ..\src\common/Defines.h ..\src\tankai/TankAIStrings.h ..\src\common/FileLines.h ..\src\tankai\TankAIStrings.cpp
..\src\scorched\release\srcGLWGLWFileViewscorched.obj: ..\src\common/Defines.h ..\src\GLW\GLWFileView.cpp ..\src\GLEXT/GLImage.h ..\src\GLW/GLWButton.h ..\src\common/FileLines.h ..\src\common/Vector.h ..\src\GLW/GLWFileView.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWScrollButton.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFont2d.h ..\src\GLW/GLWScrollW.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLW/GLWFont.h
..\src\scorched\release\srcclientWindowSetupscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWCheckBox.h ..\src\dialogs/PlanViewDialog.h ..\src\dialogs/BuyAccessoryDialog.h ..\src\GLEXT/GLImage.h ..\src\GLW/GLWPushButton.h ..\src\dialogs/ScoreDialog.h ..\src\GLW/GLWScrollPanel.h ..\src\weapons/Weapon.h ..\src\GLW/GLWButton.h ..\src\dialogs/PlayersDialog.h ..\src\GLW/GLWTextBox.h ..\src\dialogs/OptionsDialog.h ..\src\common/FileLines.h ..\src\GLW/GLWFlag.h ..\src\dialogs/KibitzingDialog.h ..\src\GLW/GLWLabel.h ..\src\dialogs/KillDialog.h ..\src\common/Vector.h ..\src\GLW/GLWListView.h ..\src\GLW/GLWDropDown.h ..\src\dialogs/PlayerDialog.h ..\src\GLW/GLWWindow.h ..\src\GLW/GLWScrollButton.h ..\src\GLEXT/GLTexture.h ..\src\dialogs/ConnectDialog.h ..\src\GLW/GLWidget.h ..\src\dialogs/HelpDialog.h ..\src\GLW/GLWSpinBox.h ..\src\tankgraph/GLWTankViewer.h ..\src\dialogs/QuitDialog.h ..\src\GLEXT/GLVertexArray.h ..\src\dialogs/WindDialog.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLMenu.h ..\src\client/WindowSetup.h ..\src\dialogs/TankDialog.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLW/GLWTextButton.h ..\src\common/WindowManager.h ..\src\GLEXT/GLState.h ..\src\dialogs/MainMenuDialog.h ..\src\engine/MainLoopI.h ..\src\GLW/GLWTab.h ..\src\GLEXT/GLFont2d.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\GLW/GLWScrollW.h ..\src\common/Keyboard.h ..\src\weapons/Accessory.h ..\src\dialogs/BackdropDialog.h ..\src\GLW/GLWVisibleWidget.h ..\src\client/ClientState.h ..\src\coms/NetBuffer.h ..\src\client\WindowSetup.cpp ..\src\dialogs/TalkDialog.h ..\src\engine/GameStateI.h ..\src\GLEXT/GLMenuEntry.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srclandscapeWaterMapSurroundscorched.obj: ..\src\common/Defines.h ..\src\common/Vector.h ..\src\landscape\WaterMapSurround.cpp ..\src\GLEXT/GLState.h ..\src\landscape/WaterMapSurround.h
..\src\scorched\release\srclandscapeTriNodescorched.obj: ..\src\landscape\TriNode.cpp ..\src\landscape/TriNode.h
..\src\scorched\release\srcclientMain2DCamerascorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLCamera.h ..\src\client/MainCamera.h ..\src\client\Main2DCamera.cpp ..\src\client/Main2DCamera.h ..\src\common/Vector.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\engine/GameState.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameStateI.h ..\src\common/Line.h
..\src\scorched\release\srcGLWGLWScrollPanelscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWScrollPanel.h ..\src\GLW/GLWButton.h ..\src\GLW/GLWScrollButton.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLEXT/GLState.h ..\src\GLW\GLWScrollPanel.cpp ..\src\GLW/GLWScrollW.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcscorchedmainscorched.obj: ..\src\common/Defines.h ..\src\common/ARGParser.h ..\src\scorched\main.cpp ..\src\common/FileLines.h ..\src\common/Vector.h ..\src\common/OptionEntry.h ..\src\common/OptionsTransient.h ..\src\client/OptionsClient.h ..\src\common/Registry.h ..\src\client/ClientMain.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srccomsComsStartGameMessagescorched.obj: ..\src\coms\ComsStartGameMessage.cpp ..\src\common/Vector.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\common/OptionsTransient.h ..\src\coms/ComsStartGameMessage.h ..\src\coms/NetBuffer.h ..\src\coms/ComsPlayerStateMessage.h
..\src\scorched\release\srctankgraphGLWTankViewerscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLW/GLWButton.h ..\src\common/Vector.h ..\src\tankgraph/TankModelStore.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWScrollButton.h ..\src\GLW/GLWidget.h ..\src\tankgraph/GLWTankViewer.h ..\src\GLEXT/GLVertexArray.h ..\src\common/KeyboardHistory.h ..\src\tank/TankModelId.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFont2d.h ..\src\GLW/GLWScrollW.h ..\src\tankgraph/TankMesh.h ..\src\tankgraph\GLWTankViewer.cpp ..\src\GLW/GLWVisibleWidget.h ..\src\GLW/GLWFont.h ..\src\tankgraph/TankModel.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcGLWGLWVisiblePanelscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWidget.h ..\src\GLW\GLWVisiblePanel.cpp ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLEXT/GLState.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcodefastlsolvescorched.obj: ..\src\ode\fastlsolve.c
..\src\scorched\release\srcserverServerShotHolderscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\server\ServerShotHolder.cpp ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\coms/ComsPlayedMoveMessage.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\server/ServerShotHolder.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srccomsNetBufferscorched.obj: ..\src\coms\NetBuffer.cpp ..\src\coms/NetBuffer.h
..\src\scorched\release\srclandscapePatchVarscorched.obj: ..\src\common/Defines.h ..\src\landscape\PatchVar.cpp ..\src\GLEXT/GLVar.h ..\src\GLEXT/GLState.h ..\src\landscape/PatchVar.h
..\src\scorched\release\srcGLWGLWFlagscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWFlag.h ..\src\common/Vector.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLW\GLWFlag.cpp ..\src\GLW/GLWVisibleWidget.h
..\src\scorched\release\srccomsComsRmPlayerMessagescorched.obj: ..\src\coms/ComsRmPlayerMessage.h ..\src\coms\ComsRmPlayerMessage.cpp ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srcspritesSmokeActionRendererscorched.obj: ..\src\common/Defines.h ..\src\engine/ActionMeta.h ..\src\ode/memory.h ..\src\landscape/Variance.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\GLEXT/GLImage.h ..\src\engine/PhysicsParticleObject.h ..\src\GLEXT/GLTextureCubeMap.h ..\src\landscape/ShadowMap.h ..\src\landscape/PatchGrid.h ..\src\sprites/SmokeActionRenderer.h ..\src\sprites\SmokeActionRenderer.cpp ..\src\landscape/HeightMap.h ..\src\landscape/Surround.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\landscape/Wall.h ..\src\landscape/WaterMapPoints.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\GLEXT/GLTexture.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\landscape/Smoke.h ..\src\ode/matrix.h ..\src\landscape/Landscape.h ..\src\landscape/WaterMap.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\landscape/SmokeChain.h ..\src\landscape/Sun.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\landscape/TriNode.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\landscape/TriNodePool.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\engine/GameState.h ..\src\landscape/Patch.h ..\src\landscape/WaterMapSurround.h ..\src\engine/PhysicsParticle.h ..\src\ode/odecpp_collision.h ..\src\landscape/SurroundDefs.h ..\src\GLEXT/GLBitmap.h ..\src\landscape/HeightMapSurround.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\landscape/PatchSide.h ..\src\engine/Action.h
..\src\scorched\release\srcGLWGLWScrollButtonscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWButton.h ..\src\GLW/GLWScrollButton.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLW\GLWScrollButton.cpp ..\src\GLW/GLWVisibleWidget.h
..\src\scorched\release\srccommonLinescorched.obj: ..\src\common\Line.cpp ..\src\common/Vector.h ..\src\common/Line.h
..\src\scorched\release\srcserverServerMessageHandlerscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\server/ServerMessageHandler.h ..\src\coms/ComsRmPlayerMessage.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\coms/ComsMessageHandler.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\server\ServerMessageHandler.cpp ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsMessageSender.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\server/ServerDialog.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srclandscapeCloudSimscorched.obj: ..\src\common/Defines.h ..\src\landscape/CloudSim.h ..\src\GLEXT/GLState.h ..\src\landscape\CloudSim.cpp
..\src\scorched\release\srczlibadler32scorched.obj: ..\src\zlib\adler32.c
..\src\scorched\release\srctankTankscorched.obj: ..\src\tank/TankShields.h ..\src\tank/TankWeapon.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\ode/rotation.h ..\src\tank\Tank.cpp ..\src\tank/TankBatteries.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\weapons/Weapon.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\ode/error.h ..\src\tank/TankState.h ..\src\ode/ode.h ..\src\ode/common.h ..\src\ode/odecpp_collision.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/misc.h ..\src\tank/TankColorGenerator.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\ode/odecpp.h ..\src\engine/ScorchedCollisionIds.h
..\src\scorched\release\srctankTankContainerscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\tank\TankContainer.cpp
..\src\scorched\release\srctankTankLibscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\tank/TankLib.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\tank\TankLib.cpp
..\src\scorched\release\srcGLEXTGLBitmapItteratorscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLBitmapItterator.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLBitmap.h ..\src\GLEXT\GLBitmapItterator.cpp
..\src\scorched\release\srcdialogsOptionsDialogscorched.obj: ..\src\engine/ActionMeta.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\GLW/GLWCheckBox.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\common/OptionsGame.h ..\src\common/ARGParser.h ..\src\GLW/GLWPushButton.h ..\src\GLW/GLWScrollPanel.h ..\src\GLW/GLWButton.h ..\src\engine/ActionBuffer.h ..\src\common/FileLines.h ..\src\dialogs/OptionsDialog.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\GLW/GLWLabel.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\GLW/GLWDropDown.h ..\src\ode/misc.h ..\src\GLW/GLWWindow.h ..\src\ode/collision_space.h ..\src\GLW/GLWScrollButton.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\GLW/GLWidget.h ..\src\ode/matrix.h ..\src\GLW/GLWSpinBox.h ..\src\common/OptionEntry.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\ode/collision.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLW/GLWTextButton.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\dialogs\OptionsDialog.cpp ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\engine/MainLoop.h ..\src\GLW/GLWTab.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\GLW/GLWScrollW.h ..\src\common/Timer.h ..\src\ode/odecpp_collision.h ..\src\GLW/GLWVisibleWidget.h ..\src\engine/ActionController.h ..\src\client/ClientState.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\engine/Action.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcclientClientConnectionAcceptHandlerscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\GLW/GLWButton.h ..\src\GLW/GLWTextBox.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\GLW/GLWLabel.h ..\src\common/Vector.h ..\src\GLW/GLWListView.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\GLW/GLWWindow.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\GLW/GLWScrollButton.h ..\src\dialogs/ConnectDialog.h ..\src\ode/odecpp_old_collision.h ..\src\GLW/GLWidget.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\coms/ComsConnectAcceptMessage.h ..\src\tank/TankParachutes.h ..\src\coms/ComsMessageHandler.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLW/GLWTextButton.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\ode/timer.h ..\src\client\ClientConnectionAcceptHandler.cpp ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\GLW/GLWScrollW.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\GLW/GLWVisibleWidget.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\client/ClientConnectionAcceptHandler.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcdialogsPlayersDialogscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\common/OptionsGame.h ..\src\tank/TankBatteries.h ..\src\common/ARGParser.h ..\src\GLW/GLWPushButton.h ..\src\weapons/Weapon.h ..\src\GLW/GLWButton.h ..\src\dialogs/PlayersDialog.h ..\src\tankai/TankAIComputer.h ..\src\GLW/GLWTextBox.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\GLW/GLWFlag.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\GLW/GLWLabel.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\GLW/GLWDropDown.h ..\src\GLW/GLWWindow.h ..\src\ode/misc.h ..\src\coms/NetMessageHandler.h ..\src\ode/collision_space.h ..\src\dialogs\PlayersDialog.cpp ..\src\GLW/GLWScrollButton.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\GLW/GLWidget.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\tankgraph/GLWTankViewer.h ..\src\GLEXT/GLVertexArray.h ..\src\coms/ComsMessageHandler.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\GLW/GLWVisiblePanel.h ..\src\common/WindowManager.h ..\src\GLW/GLWTextButton.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tankai/TankAIStore.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\GLW/GLWScrollW.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\GLW/GLWVisibleWidget.h ..\src\client/ClientState.h ..\src\tank/TankAutoDefense.h ..\src\tankai/TankAIComputerBuyer.h ..\src\coms/NetBuffer.h ..\src\tank/TankColorGenerator.h ..\src\engine/GameStateI.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\tankai/TankAIHuman.h ..\src\client/ClientNewGameHandler.h ..\src\ode/odecpp.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcGLEXTGLConsoleRulescorched.obj: ..\src\GLEXT\GLConsoleRule.cpp ..\src\GLEXT/GLConsoleRule.h
..\src\scorched\release\srccomsNetClientscorched.obj: ..\src\coms/NetMessagePool.h ..\src\coms/NetMessage.h ..\src\coms/NetClient.h ..\src\coms/NetMessageHandler.h ..\src\coms/NetBufferUtil.h ..\src\coms/NetBuffer.h ..\src\coms\NetClient.cpp
..\src\scorched\release\srcGLEXTGLMenuscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\common/Vector.h ..\src\GLW/GLWWindow.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLMenu.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFont2d.h ..\src\GLEXT/GLMenuI.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLW/GLWFont.h ..\src\GLEXT/GLMenuEntry.h ..\src\GLEXT\GLMenu.cpp ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcodeobstackscorched.obj: ..\src\ode/common.h ..\src\ode/memory.h ..\src\ode/config.h ..\src\ode\obstack.cpp ..\src\ode/error.h
..\src\scorched\release\srcclientClientMessageHandlerscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWButton.h ..\src\GLW/GLWTextBox.h ..\src\common/FileLines.h ..\src\GLW/GLWLabel.h ..\src\common/Vector.h ..\src\GLW/GLWListView.h ..\src\coms/NetMessage.h ..\src\GLW/GLWWindow.h ..\src\coms/NetMessageHandler.h ..\src\GLW/GLWScrollButton.h ..\src\dialogs/ConnectDialog.h ..\src\GLW/GLWidget.h ..\src\coms/ComsMessageHandler.h ..\src\client\ClientMessageHandler.cpp ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLW/GLWTextButton.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFontBanner.h ..\src\engine/MainLoopI.h ..\src\engine/GameState.h ..\src\GLW/GLWScrollW.h ..\src\GLW/GLWVisibleWidget.h ..\src\client/MainBanner.h ..\src\client/ClientState.h ..\src\coms/NetBuffer.h ..\src\client/ClientMessageHandler.h ..\src\engine/GameStateI.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcdialogsBackdropDialogscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLW/GLWWindow.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\GLEXT/GLState.h ..\src\dialogs/BackdropDialog.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLEXT/GLBitmap.h ..\src\dialogs\BackdropDialog.cpp ..\src\GLW/GLWPanel.h
..\src\scorched\release\srcclientClientMainscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLCamera.h ..\src\client/MainCamera.h ..\src\common/OptionsGame.h ..\src\common/ARGParser.h ..\src\common/Mouse.h ..\src\common/FileLines.h ..\src\common/Display.h ..\src\common/Vector.h ..\src\coms/NetMessage.h ..\src\coms/NetClient.h ..\src\coms/NetMessageHandler.h ..\src\common/OptionEntry.h ..\src\common/Sound.h ..\src\common/Gamma.h ..\src\common/KeyboardHistory.h ..\src\client/ClientDialog.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\client\ClientMain.cpp ..\src\engine/MainLoop.h ..\src\common/OptionsTransient.h ..\src\client/OptionsClient.h ..\src\engine/GameState.h ..\src\GLEXT/GLMenuI.h ..\src\common/Keyboard.h ..\src\common/Timer.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\common/Line.h
..\src\scorched\release\srccommonOptionsTransientscorched.obj: ..\src\common/Defines.h ..\src\common/OptionsGame.h ..\src\common/ARGParser.h ..\src\common\OptionsTransient.cpp ..\src\common/FileLines.h ..\src\common/Vector.h ..\src\common/OptionEntry.h ..\src\common/OptionsTransient.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srctankgraphTracerStorescorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tankgraph\TracerStore.cpp ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tankgraph/TracerStore.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srcGLEXTGLStateExtensionscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLStateExtension.h ..\src\GLEXT/GLState.h ..\src\GLEXT\GLStateExtension.cpp
..\src\scorched\release\srcserverServerStatescorched.obj: ..\src\common/Vector.h ..\src\engine/GameStateStimulusI.h ..\src\server/ServerWaitingForPlayersState.h ..\src\server/ServerPlayingState.h ..\src\server/ServerScoreState.h ..\src\common/KeyboardHistory.h ..\src\server/ServerNextRoundState.h ..\src\tankai/TankAIComputerCtrl.h ..\src\engine/MainLoopI.h ..\src\engine/GameState.h ..\src\server/ServerShotState.h ..\src\server/ServerState.h ..\src\server/ServerTooFewPlayersStimulus.h ..\src\engine/GameStateI.h ..\src\server/ServerNewGameState.h ..\src\server\ServerState.cpp
..\src\scorched\release\srccomsComsPlayerReadyMessagescorched.obj: ..\src\coms/ComsPlayerReadyMessage.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms/NetBuffer.h ..\src\coms\ComsPlayerReadyMessage.cpp
..\src\scorched\release\srcclientClientScoreHandlerscorched.obj: ..\src\common/Vector.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\coms/NetMessageHandler.h ..\src\client\ClientScoreHandler.cpp ..\src\coms/ComsMessageHandler.h ..\src\client/ClientScoreHandler.h ..\src\engine/MainLoopI.h ..\src\engine/GameState.h ..\src\coms/ComsScoreMessage.h ..\src\client/ClientState.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srcGLWGLWDropDownscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLW/GLWPushButton.h ..\src\GLW/GLWButton.h ..\src\common/Vector.h ..\src\GLW\GLWDropDown.cpp ..\src\GLW/GLWDropDown.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFont2d.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLW/GLWFont.h
..\src\scorched\release\srcodeerrorscorched.obj: ..\src\ode\error.cpp ..\src\ode/config.h ..\src\ode/error.h
..\src\scorched\release\srcGLEXTGLLuminancescorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLEXT\GLLuminance.cpp ..\src\GLEXT/GLState.h ..\src\GLEXT/GLLuminance.h
..\src\scorched\release\srclandscapeHeightMapRendererscorched.obj: ..\src\common/Defines.h ..\src\landscape\HeightMapRenderer.cpp ..\src\landscape/HeightMap.h ..\src\common/Vector.h ..\src\landscape/HeightMapRenderer.h ..\src\GLEXT/GLState.h ..\src\common/ProgressCounter.h
..\src\scorched\release\srcGLEXTGLBitmapscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLEXT/GLState.h ..\src\GLEXT\GLBitmap.cpp ..\src\GLEXT/GLBitmap.h
..\src\scorched\release\srcscorchedMainDialogscorched.obj: ..\src\scorched/MainDialog.h ..\src\common/Vector.h ..\src\scorched/DisplayDialog.h ..\src\common/OptionsTransient.h ..\src\scorched/NetDialog.h ..\src\scorched\MainDialog.cpp ..\src\coms/NetBuffer.h
..\src\scorched\release\srcserverOptionsServerscorched.obj: ..\src\common/Defines.h ..\src\common/OptionsGame.h ..\src\common/ARGParser.h ..\src\server/OptionsServer.h ..\src\server\OptionsServer.cpp ..\src\common/FileLines.h ..\src\common/OptionEntry.h ..\src\coms/NetBuffer.h
..\src\scorched\release\srccommonRegistryscorched.obj: ..\src\common/Registry.h ..\src\common\Registry.cpp
..\src\scorched\release\srcspritesTankDeadRendererscorched.obj: ..\src\common/SoundStore.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\GLW/GLWidget.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\sprites/TankDeadRenderer.h ..\src\common/Sound.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFontBanner.h ..\src\common/SoundBuffer.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\sprites\TankDeadRenderer.cpp ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\client/MainBanner.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\engine/Action.h
..\src\scorched\release\srcserverServerMainscorched.obj: ..\src\tank/TankShields.h ..\src\engine/ActionMeta.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\common/OptionsGame.h ..\src\server/OptionsServer.h ..\src\common/ARGParser.h ..\src\tankai/TankAIStrings.h ..\src\server/ServerPlayerReadyHandler.h ..\src\weapons/Weapon.h ..\src\server/ServerMessageHandler.h ..\src\tankai/TankAIComputer.h ..\src\landscape/HeightMap.h ..\src\server/ServerDefenseHandler.h ..\src\landscape/GlobalHMap.h ..\src\engine/ActionBuffer.h ..\src\common/FileLines.h ..\src\coms/NetServer.h ..\src\server/ServerConnectHandler.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\engine/ScorchedCollisionHandler.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\landscape/HeightMapCollision.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\coms/ComsMessageHandler.h ..\src\common/OptionEntry.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\coms/ComsLevelMessage.h ..\src\tank/TankContainer.h ..\src\common/ProgressCounter.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\server/ServerPlayedMoveHandler.h ..\src\engine/PhysicsEngine.h ..\src\tankai/TankAIStore.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\common/OptionsTransient.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\engine/GameState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\common/Timer.h ..\src\ode/odecpp_collision.h ..\src\server/ServerState.h ..\src\engine/ActionController.h ..\src\server/ServerTextHandler.h ..\src\tank/TankAutoDefense.h ..\src\tankai/TankAIComputerBuyer.h ..\src\coms/NetBuffer.h ..\src\tank/TankColorGenerator.h ..\src\server/ServerDialog.h ..\src\weapons/AccessoryStore.h ..\src\engine/GameStateI.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\server\ServerMain.cpp ..\src\server/ServerBuyAccessoryHandler.h ..\src\engine/Action.h
..\src\scorched\release\srcdialogsTalkDialogscorched.obj: ..\src\common/Defines.h ..\src\GLW/GLWButton.h ..\src\GLW/GLWTextBox.h ..\src\dialogs\TalkDialog.cpp ..\src\coms/ComsTextMessage.h ..\src\common/Vector.h ..\src\GLW/GLWLabel.h ..\src\coms/NetMessage.h ..\src\coms/ComsMessage.h ..\src\GLW/GLWWindow.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLW/GLWVisiblePanel.h ..\src\common/WindowManager.h ..\src\GLW/GLWTextButton.h ..\src\GLEXT/GLState.h ..\src\engine/MainLoopI.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\coms/ComsMessageSender.h ..\src\GLW/GLWVisibleWidget.h ..\src\coms/NetBuffer.h ..\src\engine/GameStateI.h ..\src\dialogs/TalkDialog.h ..\src\GLW/GLWPanel.h
..\src\scorched\release\srctankaiTankAIComputerDHTestscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\tankai/TankAIComputerDHTest.h ..\src\tankai/TankAIComputer.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\tankai/TankAIComputerTosser.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tankai/TankAIComputerPShark.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tankai/TankAIComputerShooter.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\tank/TankAutoDefense.h ..\src\tankai/TankAIComputerBuyer.h ..\src\coms/NetBuffer.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h ..\src\tankai/TankAIComputerChooser.h ..\src\tankai\TankAIComputerDHTest.cpp
..\src\scorched\release\srcclientMainBannerscorched.obj: ..\src\GLEXT/GLCamera.h ..\src\client/MainCamera.h ..\src\common/Defines.h ..\src\common/Vector.h ..\src\client\MainBanner.cpp ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFontBanner.h ..\src\engine/MainLoopI.h ..\src\GLEXT/GLMenuI.h ..\src\engine/GameState.h ..\src\client/MainBanner.h ..\src\engine/GameStateI.h ..\src\common/Line.h
..\src\scorched\release\srcweaponsWeaponscorched.obj: ..\src\common/Defines.h ..\src\weapons/Weapon.h ..\src\weapons\Weapon.cpp ..\src\weapons/Accessory.h ..\src\coms/NetBuffer.h ..\src\weapons/AccessoryStore.h
..\src\scorched\release\srcodematscorched.obj: ..\src\ode/common.h ..\src\ode/memory.h ..\src\ode/config.h ..\src\ode/matrix.h ..\src\ode/misc.h ..\src\ode/error.h ..\src\ode\mat.cpp
..\src\scorched\release\srctankgraphTankMeshscorched.obj: ..\src\common/Defines.h ..\src\common/Vector.h ..\src\GLEXT/GLVertexArray.h ..\src\3dsparse/ASEFile.h ..\src\tankgraph\TankMesh.cpp ..\src\GLEXT/GLState.h ..\src\tankgraph/TankMesh.h ..\src\3dsparse/ASEArrayFact.h ..\src\GLEXT/GLVertexTexArray.h ..\src\3dsparse/ASEModel.h ..\src\common/Face.h
..\src\scorched\release\srcweaponsBatteryscorched.obj: ..\src\common/Defines.h ..\src\common/VectorLib.h ..\src\common/Vector.h ..\src\weapons\Battery.cpp ..\src\weapons/Accessory.h ..\src\weapons/Battery.h
..\src\scorched\release\srcweaponsShieldReflectiveMagscorched.obj: ..\src\common/Vector.h ..\src\weapons\ShieldReflectiveMag.cpp ..\src\weapons/Accessory.h ..\src\weapons/Shield.h ..\src\weapons/ShieldReflective.h ..\src\weapons/ShieldReflectiveMag.h
..\src\scorched\release\srczlibinfutilscorched.obj: ..\src\zlib\infutil.c
..\src\scorched\release\srctankTankAccessoriesscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\common/Vector.h ..\src\tank/TankParachutes.h ..\src\tank/TankWeapon.h ..\src\tank\TankAccessories.cpp ..\src\tank/TankAccessories.h ..\src\weapons/Accessory.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\weapons/AccessoryStore.h ..\src\weapons/Shield.h
..\src\scorched\release\srcGLWGLWTextBoxscorched.obj: ..\src\common/Defines.h ..\src\GLEXT/GLImage.h ..\src\GLW/GLWTextBox.h ..\src\common/Vector.h ..\src\GLEXT/GLTexture.h ..\src\GLW/GLWidget.h ..\src\common/KeyboardHistory.h ..\src\GLEXT/GLState.h ..\src\GLEXT/GLFont2d.h ..\src\common/Keyboard.h ..\src\GLW/GLWVisibleWidget.h ..\src\GLW/GLWFont.h ..\src\GLW\GLWTextBox.cpp
..\src\scorched\release\srcoderotationscorched.obj: ..\src\ode/common.h ..\src\ode/rotation.h ..\src\ode\rotation.cpp ..\src\ode/odemath.h ..\src\ode/config.h ..\src\ode/error.h
..\src\scorched\release\srctankTankAIscorched.obj: ..\src\common/SoundStore.h ..\src\engine/ActionMeta.h ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\common/OptionsGame.h ..\src\tank/TankBatteries.h ..\src\common/ARGParser.h ..\src\weapons/Weapon.h ..\src\engine/ActionBuffer.h ..\src\common/FileLines.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\actions/TankResign.h ..\src\ode/contact.h ..\src\coms/ComsMessage.h ..\src\tank/Tank.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\common/OptionEntry.h ..\src\common/Sound.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\common/KeyboardHistory.h ..\src\tank\TankAI.cpp ..\src\ode/objects.h ..\src\ode/collision.h ..\src\tank/TankModelId.h ..\src\weapons/Parachute.h ..\src\coms/ComsPlayedMoveMessage.h ..\src\engine/MainLoopI.h ..\src\common/SoundBuffer.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\ode/config.h ..\src\coms/ComsDefenseMessage.h ..\src\common/OptionsTransient.h ..\src\tank/TankScore.h ..\src\tank/TankPhysics.h ..\src\engine/GameState.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsMessageSender.h ..\src\server/ServerShotHolder.h ..\src\engine/ActionController.h ..\src\client/ClientState.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\weapons/Battery.h ..\src\engine/GameStateI.h ..\src\weapons/AccessoryStore.h ..\src\weapons/Shield.h ..\src\ode/odemath.h ..\src\ode/odecpp.h ..\src\actions/TankFired.h ..\src\engine/Action.h
..\src\scorched\release\srcserverServerDefenseHandlerscorched.obj: ..\src\tank/TankShields.h ..\src\common/Defines.h ..\src\ode/memory.h ..\src\ode/geom.h ..\src\ode/rotation.h ..\src\tank/TankBatteries.h ..\src\weapons/Weapon.h ..\src\server/ServerDefenseHandler.h ..\src\ode/error.h ..\src\ode/common.h ..\src\ode/ode.h ..\src\common/Vector.h ..\src\ode/space.h ..\src\coms/NetMessage.h ..\src\ode/contact.h ..\src\tank/Tank.h ..\src\coms/ComsMessage.h ..\src\ode/misc.h ..\src\ode/collision_space.h ..\src\coms/NetMessageHandler.h ..\src\ode/odecpp_old_collision.h ..\src\ode/mass.h ..\src\ode/matrix.h ..\src\tank/TankParachutes.h ..\src\coms/ComsMessageHandler.h ..\src\engine/ScorchedCollisionIds.h ..\src\tank/TankWeapon.h ..\src\ode/objects.h ..\src\tank/TankModelId.h ..\src\ode/collision.h ..\src\tank/TankContainer.h ..\src\coms/ComsPlayedMoveMessage.h ..\src\engine/MainLoopI.h ..\src\ode/timer.h ..\src\engine/PhysicsEngine.h ..\src\tank/TankAccessories.h ..\src\coms/ComsDefenseMessage.h ..\src\ode/config.h ..\src\server\ServerDefenseHandler.cpp ..\src\engine/GameState.h ..\src\tank/TankPhysics.h ..\src\tank/TankScore.h ..\src\tank/TankAI.h ..\src\weapons/Accessory.h ..\src\tank/TankState.h ..\src\ode/odecpp_collision.h ..\src\coms/ComsMessageSender.h ..\src\server/ServerState.h ..\src\server/ServerShotHolder.h ..\src\tank/TankAutoDefense.h ..\src\coms/NetBuffer.h ..\src\weapons/AccessoryStore.h ..\src\ode/odemath.h ..\src\weapons/Shield.h ..\src\ode/odecpp.h
..\src\scorched\release\srcGLEXTGLConsoleRuleMethodscorched.obj: ..\src\GLEXT\GLConsoleRuleMethod.cpp ..\src\GLEXT/GLConsoleRuleMethod.h ..\src\GLEXT/GLConsoleRule.h
..\src\scorched\release\srccommonARGParserscorched.obj: ..\src\common/Defines.h ..\src\common/ARGParser.h ..\src\common\ARGParser.cpp
