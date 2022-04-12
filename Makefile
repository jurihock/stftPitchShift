.PHONY: help boot build install install-test reinstall uninstall upload upload-test which

help:
	@echo boot
	@echo build
	@echo install
	@echo install-test
	@echo reinstall
	@echo uninstall
	@echo upload
	@echo upload-test
	@echo which

boot:
	@python3 -m pip install --upgrade build
	@python3 -m pip install --upgrade twine

build:
	@rm -rf dist
	@python3 -m build

install:
	@python3 -m pip install stftpitchshift

install-test:
	@python3 -m pip install --index-url https://test.pypi.org/simple/ --no-deps stftpitchshift

reinstall:
	@python3 -m pip uninstall -y stftpitchshift
	@python3 -m pip install stftpitchshift

uninstall:
	@python3 -m pip uninstall -y stftpitchshift

upload:
	@python3 -m twine upload dist/*

upload-test:
	@python3 -m twine upload --repository testpypi dist/*

which:
	@which python3
