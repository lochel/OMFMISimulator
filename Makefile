RM=rm -rf
MKDIR=mkdir -p

.PHONY: OMSimulator config config-OMSimulator config-fmil config-lua config-cvode config-kinsol config-3rdParty distclean testsuite

OMSimulator:
	@echo
	@echo "# make OMSimulator"
	@echo
	@$(MAKE) -C build/linux install

config: config-OMSimulator
config-3rdParty: config-fmil config-lua config-cvode config-kinsol

config-OMSimulator:
	@echo
	@echo "# config OMSimulator"
	@echo
	$(RM) build/linux
	$(MKDIR) build/linux
	cd build/linux && cmake ../.. && $(MAKE) install

config-fmil:
	@echo
	@echo "# config fmil"
	@echo
	$(RM) 3rdParty/FMIL/build/linux
	$(RM) 3rdParty/FMIL/install/linux
	$(MKDIR) 3rdParty/FMIL/build/linux
	cd 3rdParty/FMIL/build/linux && cmake -DFMILIB_INSTALL_PREFIX=../../install/linux ../.. && $(MAKE) install

config-lua:
	@echo
	@echo "# config Lua"
	@echo
	$(RM) 3rdParty/Lua/install/linux
	@$(MAKE) -C 3rdParty/Lua

config-cvode:
	@echo
	@echo "# config cvode"
	@echo
	$(RM) 3rdParty/cvode/build-linux
	$(RM) 3rdParty/cvode/install/linux
	$(MKDIR) 3rdParty/cvode/build-linux
	cd 3rdParty/cvode/build-linux && cmake -DCMAKE_INSTALL_PREFIX=../install/linux .. -DEXAMPLES_ENABLE:BOOL="0" -DBUILD_SHARED_LIBS:BOOL="0" -DCMAKE_C_FLAGS="-fPIC" && $(MAKE) install

config-kinsol:
	@echo
	@echo "# config kinsol"
	@echo
	$(RM) 3rdParty/kinsol/build-linux
	$(RM) 3rdParty/kinsol/install/linux
	$(MKDIR) 3rdParty/kinsol/build-linux
	cd 3rdParty/kinsol/build-linux && cmake -DCMAKE_INSTALL_PREFIX=../install/linux .. -DEXAMPLES_ENABLE:BOOL="0" -DBUILD_SHARED_LIBS:BOOL="0" -DCMAKE_C_FLAGS="-fPIC" && $(MAKE) install

distclean:
	@echo
	@echo "# make distclean"
	@echo
	$(RM) build/linux
	$(RM) install
	$(RM) 3rdParty/FMIL/build/linux
	$(RM) 3rdParty/FMIL/install/linux
	$(RM) 3rdParty/cvode/build-linux
	$(RM) 3rdParty/cvode/install/linux

testsuite:
	@echo
	@echo "# run testsuite"
	@echo
	@$(MAKE) -C testsuite all
