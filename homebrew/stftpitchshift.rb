class Stftpitchshift < Formula
  desc "STFT based multi pitch shifting with optional formant preservation"
  homepage "https://github.com/jurihock/stftPitchShift"
  url "https://github.com/jurihock/stftPitchShift/archive/refs/tags/<VERSION>.tar.gz"
  sha256 "<SHA256>"
  license "MIT"

  depends_on "cmake" => :build
  depends_on "cmake" => :test

  def install
    mkdir "build" do
      system "cmake", "-DBUILD_SHARED_LIBS=OFF", "-DBREW=ON", "-G", "Unix Makefiles", "..", *std_cmake_args
      system "make"
      system "make", "install"
    end
  end

  test do
    system "stftpitchshift", "-h"

    (testpath/"CMakeLists.txt").write("find_package(stftpitchshift)")
    system "cmake", "."
  end
end
