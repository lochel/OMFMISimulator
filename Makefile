RM=rm -rf
MKDIR=mkdir -p

.PHONY: OMSimulator libOMSimulator fmil clean distclean testsuite

OMSimulator: libOMSimulator
	@echo
	@echo "# make OMSimulator"
	@echo
	$(MAKE) -C simulator OMSimulator

libOMSimulator:
	@echo
	@echo "# make libOMSimulator"
	@echo
	$(MAKE) -C library libOMSimulator

fmil:
	@echo
	@echo "# make fmil"
	@echo
	$(MKDIR) 3rdParty/FMIL/build-fmil
	cd 3rdParty/FMIL/build-fmil && cmake .. && $(MAKE) install

clean:
	@echo
	@echo "# make clean"
	@echo
	$(MAKE) -C library clean
	$(MAKE) -C simulator clean

distclean: clean
	@echo
	@echo "# make distclean"
	@echo
	$(MAKE) -C library distclean
	$(MAKE) -C simulator distclean
	$(RM) build
	$(RM) 3rdParty/FMIL/build-fmil
	$(RM) 3rdParty/FMIL/install

testsuite:
	@echo
	@echo "# make testsuite"
	@echo
	@$(MAKE) -C testsuite all
