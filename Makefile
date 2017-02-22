RM=rm -rf
MKDIR=mkdir -p

.PHONY: OMSimulator config config-OMSimulator config-fmil distclean testsuite

OMSimulator:
	@echo
	@echo "# make OMSimulator"
	@echo
	@$(MAKE) -C build-cmake install

config: config-fmil config-OMSimulator

config-OMSimulator:
	@echo
	@echo "# config OMSimulator"
	@echo
	$(RM) build
	$(RM) build-cmake
	$(MKDIR) build-cmake
	cd build-cmake && cmake .. && $(MAKE) install

config-fmil:
	@echo
	@echo "# config fmil"
	@echo
	$(RM) 3rdParty/FMIL/build-fmil
	$(RM) 3rdParty/FMIL/install
	$(MKDIR) 3rdParty/FMIL/build-fmil
	cd 3rdParty/FMIL/build-fmil && cmake .. && $(MAKE) install

distclean:
	@echo
	@echo "# make distclean"
	@echo
	$(RM) build
	$(RM) build-cmake
	$(RM) 3rdParty/FMIL/build-fmil
	$(RM) 3rdParty/FMIL/install

testsuite:
	@echo
	@echo "# run testsuite"
	@echo
	@$(MAKE) -C testsuite all
