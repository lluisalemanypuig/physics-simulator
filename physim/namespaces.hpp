// File used for documentation purposes only

/**
 * @brief Simulator namespace.
 *
 * Main namespace of this library.
 */
namespace physim {

    /**
     * @brief Geometry objects.
     *
     * Namespace containing geometrical objects whose position does not
     * depend on the interaction with other objects.
     */
    namespace geom { }
    
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
     * Contains the definition of the type @ref vec3.
     * 
     * Also, it contains several macros for the efficient compilation
     * of typical operations between vecors. These are not meant to be
     * used outside the library, since they are cumbersome to use.
     * 
     * However, in the files @ref math_ops.hpp, @ref math_misc.hpp,
     * and @ref vec3.hpp, all the macros have been documented (though
     * not appearing in the html documentation).
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

} // -- namespace sim
