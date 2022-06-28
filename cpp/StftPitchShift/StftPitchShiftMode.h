#pragma once

namespace stftpitchshift
{
  /**
   * The common operation modes.
   **/
  enum class StftPitchShiftMode
  {
    /**
     * Perform regular pitch shifting,
     * with or without formant preservation -q.
     **/
    pitch,

    /**
     * Perform formant shifting to change the timbre,
     * but only if formant extraction -q is enabled.
     **/
    timbre
  };
}
