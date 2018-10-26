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

} // -- namespace sim
