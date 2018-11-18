#pragma once

// C++ includes
#include <vector>
#include <string>
#include <map>

// base includes
#include <base/model/model_utils.hpp>

/**
 * @brief Single class to manage textures.
 *
 * Used to avoid allocating the same texture
 * into memory more than once, free the memory
 * occupied by the same texture more than once,
 * ...
 */
class texture_loader {
	private:

		/// The textures loaded into memory
		std::map<std::string, unsigned int> textures;
		/// Constructor
		texture_loader();

	public:
		/**
		 * @brief Destructor
		 *
		 * Calls @ref clear_all method
		 */
		~texture_loader();

		// MODIFIERS

		/**
		 * @brief Loads the textures of a collection of materials.
		 *
		 * Given a collection of materials, either withor without textures,
		 * load all of them into memory. If a texture was loaded previously
		 * then it is NOT loaded again.
		 *
		 * The indexes generated for each texture are stored in @e info.
		 * @param[out] mats Collection of materials. The textures indexes
		 * are is overriden for those materials with textures.
		 */
		void load_textures(std::vector<material>& mats);

		/**
		 * @brief Clears the textures of a collection of materials.
		 *
		 * Given a collection of materials, either withor without textures,
		 * clear all their textures. If a texture was cleared previouly,
		 * then it is NOT cleared twice.
		 * @param[in] mats Collection of materials.
		 */
		void clear_textures(const std::vector<material>& mats);

		/**
		 * @brief Clears all textures loaded so far.
		 */
		void clear_all();

		// GETTERS

		/// Returns the index of the texture.
		unsigned int texture_index(const std::string& s) const;

		/// Return the texture loader
		static texture_loader& get_loader() {
			static texture_loader L;
			return L;
		}

		/// Deleted copy-constructor.
		texture_loader (const texture_loader& L) = delete;
		/// Deleted assignation operator.
		void operator= (const texture_loader& L) = delete;

};
