void core_test() {
  typedef fuse::make_seq<int>::type seq1;
  typedef fuse::make_seq<int, char>::type seq2;
  typedef fuse::make_seq<int, char, bool>::type seq3;
  typedef fuse::make_seq<int, char, bool, short>::type seq4;

  STATIC_TEST("CanCreateSingletonSeq", (
    std::is_same<fuse::seq<int>::head, int>::value &&
    std::is_same<fuse::seq<int>::tail, fuse::empty_seq>::value
  ));

  STATIC_TEST("CanCreateTwoSeq", (
    std::is_same<fuse::seq<int, fuse::seq<char>>::head, int>::value &&
    std::is_same<fuse::seq<int, fuse::seq<char>>::tail, fuse::seq<char>>::value &&
    std::is_same<fuse::seq<int, fuse::seq<char>>::tail::head, char>::value &&
    std::is_same<fuse::seq<int, fuse::seq<char>>::tail::tail, fuse::empty_seq>::value
  ));

  STATIC_TEST("CanMakeSeq", (
    std::is_same<seq4::head, int>::value &&
    std::is_same<seq4::tail::head, char>::value &&
    std::is_same<seq4::tail::tail::head, bool>::value &&
    std::is_same<seq4::tail::tail::tail::head, short>::value
  ));

  STATIC_TEST("EmptySeqsAreIgnored", (
    std::is_same<
      fuse::make_seq<fuse::empty_seq, int, fuse::empty_seq, bool, fuse::empty_seq>::type,
      fuse::make_seq<int, bool>::type
    >::value
  ));

  STATIC_TEST("EmptySeqsAreEqual", (
    fuse::equal<fuse::empty_seq, fuse::empty_seq>::value
  ));

  STATIC_TEST("SeqsWithDifferentLengthsAreUnequal", (
    !fuse::equal<fuse::make_seq<int, char, bool>::type, fuse::make_seq<int, char>::type>::value &&
    !fuse::equal<fuse::make_seq<int, char>::type, fuse::make_seq<int, char, bool>::type>::value
  ));
  
  STATIC_TEST("EqualSeqsAreEqual", (
    fuse::equal<fuse::make_seq<int, char, bool>::type, fuse::make_seq<int, char, bool>::type>::value
  ));

  STATIC_TEST("SizeOfEmptySeqIsZero", (
    fuse::size<fuse::empty_seq>::value == 0
  ));

  STATIC_TEST("CanConcatEmptySeqs", (
    std::is_same<fuse::empty_seq, fuse::concat<fuse::empty_seq, fuse::empty_seq>::type>::value
  ));

  STATIC_TEST("CanAppendToEmptySeq", (
    fuse::equal<
      fuse::concat<fuse::empty_seq, fuse::make_seq<int, char>::type>::type,
      fuse::make_seq<int, char>::type
    >::value
  ));

  STATIC_TEST("CanAppendWithEmptySeq", (
    fuse::equal<
      fuse::concat<fuse::make_seq<int, char>::type, fuse::empty_seq>::type,
      fuse::make_seq<int, char>::type
    >::value
  ));

  STATIC_TEST("CanConcatSeqs", (
    fuse::equal<
      fuse::concat<fuse::make_seq<int, char>::type, fuse::make_seq<bool, short>::type>::type,
      fuse::make_seq<int, char, bool, short>::type
    >::value
  ));

  STATIC_TEST("CanGetNonNegativeIndices", (
    fuse::equal<
      fuse::make_seq<
        fuse::get<seq4, 0>::type,
        fuse::get<seq4, 1>::type,
        fuse::get<seq4, 2>::type,
        fuse::get<seq4, 3>::type
      >::type,
      seq4
    >::value
  ));

  STATIC_TEST("CanGetNegativeIndices", (
    fuse::equal<
      fuse::make_seq<
        fuse::get<seq4, -4>::type,
        fuse::get<seq4, -3>::type,
        fuse::get<seq4, -2>::type,
        fuse::get<seq4, -1>::type
      >::type,
      seq4
    >::value
  ));

  STATIC_TEST("CanMapEmptyList", (
    std::is_same<
      fuse::map<std::add_const, fuse::empty_seq>::type,
      fuse::empty_seq
    >::value
  ));

  STATIC_TEST("CanMapAddConst", (
    fuse::equal<
      fuse::map<std::add_const, seq4>::type,
      fuse::make_seq<const int, const char, const bool, const short>::type
    >::value
  ));

  STATIC_TEST("CanReduceTwoItems", (
    std::is_same<
      fuse::reduce<std::common_type, fuse::make_seq<int, short>::type>::type,
      int
    >::value
  ));

  STATIC_TEST("CanReduceThreeItems", (
    std::is_same<
      fuse::reduce<std::common_type, fuse::make_seq<int, short, double>::type>::type,
      double
    >::value
  ));

  STATIC_TEST("CanFilterEmptyList", (
    fuse::equal<
      fuse::filter<std::is_const, fuse::empty_seq>::type,
      fuse::empty_seq
    >::value
  ));

  STATIC_TEST("CanFilterSingleItemPositive", (
    fuse::equal<
      fuse::filter<std::is_integral, seq1>::type,
      seq1
    >::value
  ));

  STATIC_TEST("CanFilterSingleItemNegative", (
    fuse::equal<
      fuse::filter<std::is_const, seq1>::type,
      fuse::empty_seq
    >::value
  ));
}
