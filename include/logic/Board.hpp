#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace core {
class Tile;
}

namespace logic {

class Board {
 private:
  std::vector<std::unique_ptr<core::Tile>> tiles_;
  std::map<std::string, int> codeIndex_;

 public:
  Board() = default;
  ~Board();

  /**
   * @brief Tambah tile ke board dan (opsional) registrasi code-nya.
   * @param tile Tile yang akan ditambahkan (ownership berpindah ke Board).
   * @param code Kode unik tile, misal "JKT", "GO". Kosong = tidak diregister.
   */
  void addTile(std::unique_ptr<core::Tile> tile, const std::string& code = "");

  /**
   * @brief Hapus semua tiles (untuk re-initialize board).
   */
  void clear();

  core::Tile* getTile(int index) const;

  /**
   * @brief Cari tile berdasarkan kode uniknya.
   * @return Pointer ke tile, atau nullptr jika tidak ditemukan.
   */
  core::Tile* getTileByCode(const std::string& code) const;

  int getTileCount() const;
};

}  // namespace logic