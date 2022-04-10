import os
import sys

python = os.path.join(os.path.dirname(__file__), 'python')

sys.path.insert(0, python)

if __name__ == '__main__':

    from stftpitchshift.main import main

    sys.exit(main())
