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
	@python -m pip install --upgrade build
	@python -m pip install --upgrade twine

build:
	@rm -rf dist
	@python -m build

install:
	@python -m pip install stftpitchshift

install-test:
	@python -m pip install --index-url https://test.pypi.org/simple/ --no-deps stftpitchshift

reinstall:
	@python -m pip uninstall -y stftpitchshift
	@python -m pip install stftpitchshift

uninstall:
	@python -m pip uninstall -y stftpitchshift

upload:
	@python -m twine upload dist/*

upload-test:
	@python -m twine upload --repository testpypi dist/*

which:
	@which python
