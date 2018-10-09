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

} // -- namespace sim
