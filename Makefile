RM=rm -rf
MKDIR=mkdir -p

.PHONY: OMSimulator config config-OMSimulator config-fmil config-lua config-3rdParty distclean testsuite

OMSimulator:
	@echo
	@echo "# make OMSimulator"
	@echo
	@$(MAKE) -C build/linux install

config: config-OMSimulator
config-3rdParty: config-fmil config-lua

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

distclean:
	@echo
	@echo "# make distclean"
	@echo
	$(RM) build/linux
	$(RM) install
	$(RM) 3rdParty/FMIL/build/linux
	$(RM) 3rdParty/FMIL/install/linux

testsuite:
	@echo
	@echo "# run testsuite"
	@echo
	@$(MAKE) -C testsuite all
