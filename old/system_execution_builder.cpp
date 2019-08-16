namespace system_execution_builder
    {
        namespace stage1
        {
            class data
            {
            };
        }

        namespace stage0
        {
            namespace keys
            {
                constexpr auto start_tags = bh::int_c<0>;
                constexpr auto start_tags_set = bh::int_c<1>;
            }

            template <typename TOptions>
            class data
            {
            private:
                template <typename TKey, typename T>
                constexpr auto change_self(const TKey& key, T&& x) const
                    noexcept
                {
                    auto new_options = TOptions{}.set(key, FWD(x));
                    return data<ECST_DECAY_DECLTYPE(new_options)>{};
                }

            public:
                // Select starting system tags.
                template <typename... TSystemTags>
                auto from(TSystemTags... sts) const noexcept
                {
                    ECST_S_ASSERT_DT(tag::system::valid(sts...));

                    // Assert that starting systems tags have not been set
                    // before.
                    ECST_S_ASSERT_DT(
                        TOptions{}.at(keys::start_tags_set) == bh::false_c);

                    return change_self(keys::start_tags, mp::list::make(sts...))
                        .change_self(keys::start_tags_set, bh::true_c);
                }

                auto match_all() const noexcept
                {
                }

                template <typename... TSystemTags>
                auto match_tags(TSystemTags... sts) const noexcept
                {
                    static_assert(sizeof...(sts) > 0);

                }

                template <typename TSystemTagList>
                auto match_tag_list(TSystemTagList stl) const noexcept
                {
                }
            };

            constexpr auto make() noexcept
            {
                auto dopts = // .
                    mp::opt_map::make()
                        .add(keys::start_tags, mp::list::empty_v)
                        .add(keys::start_tags_set, bh::false_c);


                return data<ECST_DECAY_DECLTYPE(dopts)>{};
            }
        }


        struct exec
        {
        };

        struct matcher
        {
            auto match_all()
            {
            }

            template <typename... TSystemTags>
            auto match_tags(TSystemTags... sts)
            {
            }
        };
    }
