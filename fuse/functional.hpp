#pragma once

namespace fuse {
  /**
   * Partially apply a function with some fixed arguments.
   *
   * @tparam Func The function to partially apply
   * @tparam Arg The fixed arguments, in order.
   */
  template <template <class First, class...Rest> class Func, class...Arg>
  struct apply {
    /**
     * Get the type of the partial function.
     */
    template <class...Args>
    using type = Func<Arg..., Args...>;
  };

  /**
   * Return the result of applying a function to every element of a sequence.
   *
   * @tparam Func The function to apply
   * @tparam Seq The sequence to map over
   */
  template <template <class> class Func, class Seq>
  struct map {
    /**
     * The resulting sequence of the map.
     */
    using type = seq<
      typename Func<typename Seq::head>::type,
      typename map<Func, typename Seq::tail>::type
    >;
  };

  /**
   * Base case specialisation for map. The map of an empty sequence is an
   * empty sequence.
   */
  template <template <class> class Func>
  struct map<Func, empty_seq> {
    using type = empty_seq;
  };

  /**
   * Return the result of left reducing a sequence using a binary function.
   *
   * @tparam Func The function to apply
   * @tparam Seq The sequence to reduce
   */
  template <template <class...> class Func, class Seq>
  struct reduce {
    using type =
      typename Func<typename Seq::head, typename reduce<Func, typename Seq::tail>::type>::type;
  };

  /**
   * Base case specialisation for reduce for sequences of size two.
   */
  template <template <class...> class Func, class Penultimate, class Last>
  struct reduce<Func, seq<Penultimate, seq<Last, empty_seq>>> {
    using type = typename Func<Penultimate, Last>::type;
  };

  /**
   * Get all elements of a sequence for which a predicate function is true.
   *
   * @tparam Func The predicate function
   * @tparam Seq The sequence to filter
   */
  template <template <class> class Func, class Seq>
  struct filter {
    using type = typename std::conditional<
      Func<typename Seq::head>::value,
      seq<typename Seq::head, typename filter<Func, typename Seq::tail>::type>,
      typename filter<Func, typename Seq::tail>::type
    >::type;
  };

  /**
   * Base case specialisation for filter. Filtering an empty list gives an empty list.
   */
  template <template <class> class Func>
  struct filter<Func, empty_seq> {
    using type = empty_seq;
  };
}
