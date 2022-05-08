import numpy as np


def normalize(frames, frames0):

    for i in range(len(frames)):

        a = np.real(frames0[i])
        b = np.real(frames[i])

        a = np.dot(a, a)
        b = np.dot(b, b)

        if b == 0:
            continue

        c = np.sqrt(a / b)

        frames[i] = np.real(frames[i]) * c + 1j * np.imag(frames[i])

    return frames
