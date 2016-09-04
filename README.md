


# ecst

**Experimental & work-in-progress** C++14 multithreaded compile-time [Entity-Component-System](https://en.wikipedia.org/wiki/Entity_component_system) header-only library.

Successful development of complex real-time applications and games requires a flexible and efficient entity management system. As a project becomes more intricate, it’s critical to find an elegant way to compose objects in order to prevent code repetition, improve modularity and open up powerful optimization possibilities.

The Entity-Component-System architectural pattern was designed to achieve the aforementioned benefits, by separating data from logic.

* Entities can be composed of small, reusable, and generic components.

* Components can be stored in contiguous memory areas, thus improving data locality and cache-
friendliness.

* Application logic can be easily parallelized and abstracted away from the objects themselves and
their storage policies.

* The state of the application can be serialized and shared over the network with less effort.

* A more modular, generic and easily-testable codebase.

**"ecst"** was developed as my BCS graduation project.

---------------------------------------------------------------------------------------------------

## Getting started

* [Build instructions](BUILD_INSTRUCTIONS.md)

* [Code sample](CODE_SAMPLE.md)

* [License](LICENSE)

---------------------------------------------------------------------------------------------------

## More information

* [Accompanying Thesis](https://github.com/SuperV1234/bcs_thesis)

* [Slides](https://github.com/SuperV1234/cppnow2016)

    * NOTE: These are accompanying an earlier version of the library was that presented at [**C++Now 2016**](https://cppnow2016.sched.org/event/6SgH/implementation-of-a-multithreaded-compile-time-ecs-in-c14)

--------------------------------------------------------------------------------------------------

## Terminology

* **Entities:** defined by Adam Martin (see thesis) as “fundamental conceptual building blocks” of a system, which represent concrete application ob-jects. They have no application-specific data or logic.

* **Components:** small, reusable, types that compose entities. Again, citing Adam Martin in *(see thesis)*, a component type “labels an entity as possess-ing a particular aspect”. Components store data but do not contain any logic.

* **Systems:** providers of implementation logic for entities possessing a specific set of component types.

* **Outer parallelism:** term used in ECST which defines the concept of running multiple systems that do not depend on each other in parallel. Its implementation details will be analyzed in Chapter 10 *(see thesis)*. Conceptually, an implicit directed acyclic graph is created at compile-time thanks to the knowledge of system dependencies. The execution of the implicit DAG is handled by a
system scheduler type specified during settings definition.

* **Inner parallelism:** other that running separate systems in parallel, ECST supports splitting a single system into multiple sub-tasks, which can be executed on separate threads. Many systems, such as the ones that represent functionally pure com- putations, do not contain side-effects that modify their own state or that define interactions between the subscribed entities: these are prime examples of “em-
barrassingly parallel” computations. 

---------------------------------------------------------------------------------------------------

## FAQ

* *"Where can I find documentation for the API?"*

    * See [issue #10](https://github.com/SuperV1234/ecst/issues/10).

* *"Can we have components that aren't default constructible?"*

    * This has come up before in the past. The answer is no - see [issue #8](https://github.com/SuperV1234/ecst/issues/8).

* *"I'm trying to **read/write to/from** my component, but I'm getting a compilation error. What is happening?*"

    * Systems need to know at compile-time which component types they will access and how *(read-only or write access)*. This has to be specified when defining *system signatures*. See [issue #4](https://github.com/SuperV1234/ecst/issues/4) for more info.

* *"Is it possible to iterate over the components attached to entities, without being inside of a system's process function?"*

    * See [issue #9](https://github.com/SuperV1234/ecst/issues/9#issuecomment-244577591).

* *"How do control whether my system runs in parallel, or as a single thread?*"

    * Context-wide *inner parallelism* must be enabled by calling [allow_inner_parallelism()](https://github.com/SuperV1234/ecst/blob/7f0f84a0e496d0a83ce07a41260f08528bbf79ac/include/ecst/settings/data.hpp#L118) in order to allow systems to be split in multiple *sub-tasks*. Inner parallelism strategies can be customized and composed at compile-time for particular systems, during *system signature* definition.

*  *"What is the difference between making my system single-threaded vs disabling inner paralellism?"*

    * Disabling inner parallelism prevents all systems to be split in multiple *sub-tasks*, regardless of their inner parallelism strategy. Making a system single-threaded prevents only that system from being split in multiple *sub-tasks*: this may be necessary for systems that maintain data-structures or that rely on iterating over all entities in the same thread.

