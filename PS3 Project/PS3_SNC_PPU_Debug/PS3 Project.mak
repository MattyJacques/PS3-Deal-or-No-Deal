########################################################
# PS3 PPU Makefile generated for GNU Make by ProDG VSI #
########################################################

.SILENT:

unexport MAKEFLAGS
unexport MFLAGS

OUTDIR = PS3_SNC_PPU_Debug
INTDIR = PS3_SNC_PPU_Debug

LIBS=\
	libsn.a\
	libm.a\
	libio_stub.a\
	libfs_stub.a\
	"C:/Program Files/SN Systems/PS3/ppu/lib/sn/libsn.a"\
	G:/Sony/target/ppu/lib/PSGL/RSX/debug/libPSGL.a\
	G:/Sony/target/ppu/lib/PSGL/RSX/debug/libPSGLU.a\
	G:/Sony/target/ppu/lib/libm.a\
	G:/Sony/target/ppu/lib/libusbd_stub.a\
	G:/Sony/target/ppu/lib/libfs_stub.a\
	G:/Sony/target/ppu/lib/libio_stub.a\
	G:/Sony/target/ppu/lib/libsysutil_stub.a\
	G:/Sony/target/ppu/lib/libdbgfont.a\
	G:/Sony/target/ppu/lib/libresc_stub.a\
	G:/Sony/target/ppu/lib/libgcm_cmd.a\
	G:/Sony/target/ppu/lib/libgcm_sys_stub.a\
	G:/Sony/target/ppu/lib/libsysmodule_stub.a\
	G:/Sony/target/ppu/lib/libperf.a\
	libgifdec_stub.a\
	libpngdec_stub.a\

WLIBS=\

AS=ppu-lv2-gcc
AS_FLAGS=\
	-DSN_TARGET_PS3\
	-D_DEBUG\
	-D__SNC__\
	-D__CELL_ASSERT__\
	-I.\
	-c\
	-D__GCC__\
	-g\

CC=ps3ppusnc
CC_FLAGS=\
	-DSN_TARGET_PS3\
	-D_DEBUG\
	-D__SNC__\
	-D__CELL_ASSERT__\
	-I.\
	-c\
	-Xmserrors\
	-Xdepmode=1\
	-MMD\
	-g\
	-Xc-=rtti\

CXX=ps3ppusnc
CXX_FLAGS=\
	-DSN_TARGET_PS3\
	-D_DEBUG\
	-D__SNC__\
	-D__CELL_ASSERT__\
	-I.\
	-c\
	-Xmserrors\
	-Xdepmode=1\
	-MMD\
	-g\
	-Xc-=rtti\

LD=ps3ppuld
LD_FLAGS=\
	-oformat=fself\

OBJS=\
	"PS3_SNC_PPU_Debug/AssetManager.obj"\
	"PS3_SNC_PPU_Debug/Box.obj"\
	"PS3_SNC_PPU_Debug/Camera.obj"\
	"PS3_SNC_PPU_Debug/CashNotice.obj"\
	"PS3_SNC_PPU_Debug/CollisionManager.obj"\
	"PS3_SNC_PPU_Debug/Controller.obj"\
	"PS3_SNC_PPU_Debug/Font.obj"\
	"PS3_SNC_PPU_Debug/Gameboard.obj"\
	"PS3_SNC_PPU_Debug/GraphicsManager.obj"\
	"PS3_SNC_PPU_Debug/HUD.obj"\
	"PS3_SNC_PPU_Debug/Light.obj"\
	"PS3_SNC_PPU_Debug/main.obj"\
	"PS3_SNC_PPU_Debug/Map.obj"\
	"PS3_SNC_PPU_Debug/MapReader.obj"\
	"PS3_SNC_PPU_Debug/Material.obj"\
	"PS3_SNC_PPU_Debug/MaterialFileReader.obj"\
	"PS3_SNC_PPU_Debug/Model.obj"\
	"PS3_SNC_PPU_Debug/ModelFileReader.obj"\
	"PS3_SNC_PPU_Debug/Vector.obj"\

CST_OBJS=\

CST_OUTS=\

CUSTOM: $(CST_OUTS) $(CST_OBJS)

COMPILE: $(OBJS)

"PS3_SNC_PPU_Debug/AssetManager.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/AssetManager.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/AssetManager.cpp" -o $@

"PS3_SNC_PPU_Debug/Box.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/Box.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/Box.cpp" -o $@

"PS3_SNC_PPU_Debug/Camera.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/Camera.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/Camera.cpp" -o $@

"PS3_SNC_PPU_Debug/CashNotice.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/CashNotice.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/CashNotice.cpp" -o $@

"PS3_SNC_PPU_Debug/CollisionManager.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/CollisionManager.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/CollisionManager.cpp" -o $@

"PS3_SNC_PPU_Debug/Controller.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/Controller.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/Controller.cpp" -o $@

"PS3_SNC_PPU_Debug/Font.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/Font.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/Font.cpp" -o $@

"PS3_SNC_PPU_Debug/Gameboard.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/Gameboard.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/Gameboard.cpp" -o $@

"PS3_SNC_PPU_Debug/GraphicsManager.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/GraphicsManager.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/GraphicsManager.cpp" -o $@

"PS3_SNC_PPU_Debug/HUD.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/HUD.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/HUD.cpp" -o $@

"PS3_SNC_PPU_Debug/Light.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/Light.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/Light.cpp" -o $@

"PS3_SNC_PPU_Debug/main.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/main.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/main.cpp" -o $@

"PS3_SNC_PPU_Debug/Map.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/Map.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/Map.cpp" -o $@

"PS3_SNC_PPU_Debug/MapReader.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/MapReader.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/MapReader.cpp" -o $@

"PS3_SNC_PPU_Debug/Material.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/Material.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/Material.cpp" -o $@

"PS3_SNC_PPU_Debug/MaterialFileReader.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/MaterialFileReader.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/MaterialFileReader.cpp" -o $@

"PS3_SNC_PPU_Debug/Model.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/Model.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/Model.cpp" -o $@

"PS3_SNC_PPU_Debug/ModelFileReader.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/ModelFileReader.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/ModelFileReader.cpp" -o $@

"PS3_SNC_PPU_Debug/Vector.obj":
	@echo ProDG Compiling "f:/PS3 Project/PS3 Project/Vector.cpp"
	$(CXX) $(CXX_FLAGS) "f:/PS3 Project/PS3 Project/Vector.cpp" -o $@

LINK: "PS3_SNC_PPU_Debug/PS3 Project.ppu.self"

"PS3_SNC_PPU_Debug/PS3 Project.ppu.self" : $(CST_OBJS) $(OBJS)
	@echo ProDG Linking $@
	$(LD) $(LD_FLAGS) --start-group $(CST_OBJS) $(OBJS) $(LIBS) --end-group -o $@

