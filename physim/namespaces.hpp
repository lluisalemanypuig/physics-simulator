// File used for documentation purposes only

/**
 * @brief Simulator namespace.
 *
 * Main namespace of this library.
 */
namespace physim {

    /**
     * @brief Geometrical objects.
     *
     * Namespace containing geometrical objects whose position does not
     * depend on the interaction with other objects, namely, they remain
	 * fixed on the same place throughout the simulation.
	 *
	 * Some of the geometrical objects are parametrised by their points,
	 * like @ref geometry::triangle, @ref geometry::sphere, @ref geometry::plane,
	 * @ref geometry::rectangle, but more general objects, see @ref geometry::object,
	 * are supported. These general objects must be read from a file,
	 * see namespace @ref physim::input.
     */
    namespace geometry { }
    
    /**
     * @brief Particle initialiser classes.
     * 
     * This namespace contains a 'main' class, the @ref init::initialiser
     * class that contains several, modifiable functions used to initialise
     * the different attributes of particles.
     * 
     * This namespace also contains several other classes that implement
     * the 'main' class. Each of these classes implement some of the
     * 'main' class attributes to offer nice-looking particle systems.
     */
    namespace init { }
    
    /**
     * @brief Mathematical operations namespace.
     * 
     * Contains the definition of the three different types of vectors:
     * @ref vec2, @ref vec3 and @ref vec4.
     * 
     * Also, it contains several macros for the efficient compilation
     * of typical operations between vecors. These are not meant to be
     * used outside the library, since they are cumbersome to use.
     */
    namespace math { }
    
    /**
     * @brief Force fields.
     * 
     * Contains the definition of several force fields.
     * 
     * These force fields are seen as vectorial fields in which a
     * point in space has associated a vector force. That is, a particle
     * at point of space @e p feels a force acting on it by a field.
     */
    namespace fields { }
    
    /**
     * @brief The different types of particles.
     * 
     * In this library there are some types of particles, each of which
     * are simulated differently depending on the context.
     * 
     * The types of particles implemented are:
     * - @ref particles::free_particle
     * - @ref particles::mesh_particle
     */
    namespace particles { }
    
    /**
     * @brief Spring meshes.
     * 
     * Namespace dedicated to the definition and implementation of
     * systems of springs.
     */
    namespace meshes { }
    
    /**
     * @brief Namespace for file reading.
     * 
     * It supports two formats:
     * - Wavefront. File extension: .obj.
     * - Polygon File Format (a.k.a. Stanford Triangle Format). File
     * extension: .ply.
     * - Triangle Soup Format. File extension: .soup.
     * 
     * These are meant for constructing triangular meshes for geometry
     * that cannot be parametrised.
     * 
     * The function used for reading a file should be
     * @ref read_file(const std::string&,const std::string&,geometry::object *),
     * however, if the extension of the file is known then the user can
     * call the corresponding function:
     * - @ref obj_read_file(const std::string&,const std::string&,geometry::object *) 
     * - @ref ply_read_file(const std::string&,const std::string&,geometry::object *)
     * - @ref soup_read_file(const std::string&,const std::string&,geometry::object *)
     */
    namespace input {
		
		/**
		 * @brief This namespace contains functions for private use.
		 * 
		 * There is no need to call any of these functions.
		 */
		namespace input_private {}
		
	} // -- namespace input

} // -- namespace sim
