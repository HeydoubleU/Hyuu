#include "../Hyuu.h"

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <utility>

//// too slow :(
//namespace Hyuu {
//    namespace Containers {
//        struct AMINO_ANNOTATE("Amino::Struct") map_key {
//            Amino::ulong_t hash;
//            Amino::Any value;
//
//            bool operator==(const map_key & other) const {
//                return hash == other.hash;
//            }
//        };
//    }
//}
//
//namespace std {
//    template <>
//    struct hash<Hyuu::Containers::map_key> {
//        std::size_t operator()(const Hyuu::Containers::map_key& key) const {
//            //return std::hash<Amino::long_t>()(key.hash);
//            return key.hash;
//        }
//    };
//}


struct HashPass {
    std::size_t operator()(const Amino::ulong_t& key) const {
        return key;
    }
};


namespace Hyuu {
	namespace Containers {
        namespace Map {
            class AMINO_ANNOTATE("Amino::Class") HYUU_DECL HashMap {
            public:
                std::unordered_map<Amino::ulong_t, Amino::Any, HashPass> m_map;

                HashMap() = default;

                HashMap(const HashMap & input) {
                    m_map = input.m_map;
                }

                ~HashMap() = default;
            };

            HYUU_DECL
                void map_set(HashMap& map AMINO_ANNOTATE("Amino::InOut outName=map_out"), const Amino::ulong_t& hash, const Amino::Any& value)
                AMINO_ANNOTATE("Amino::Node");

            HYUU_DECL
                bool map_get(const HashMap& map, const Amino::ulong_t& hash, Amino::Any& value)
                AMINO_ANNOTATE("Amino::Node");

            HYUU_DECL
                bool map_remove(HashMap& map AMINO_ANNOTATE("Amino::InOut outName=map_out"), const Amino::ulong_t& hash)
                AMINO_ANNOTATE("Amino::Node");

            HYUU_DECL
                bool map_remove(HashMap& map AMINO_ANNOTATE("Amino::InOut outName=map_out"), const Amino::Ptr<Amino::Array<Amino::ulong_t>>& hash)
                AMINO_ANNOTATE("Amino::Node");

            HYUU_DECL
                bool map_pop(HashMap& map AMINO_ANNOTATE("Amino::InOut outName=map_out"), const Amino::ulong_t& hash, Amino::Any& value)
                AMINO_ANNOTATE("Amino::Node");

            HYUU_DECL
                void map_hashes(const HashMap& map, Amino::Ptr<Amino::Array<Amino::ulong_t>>& hashes)
                AMINO_ANNOTATE("Amino::Node");

            HYUU_DECL
                void map_values(const HashMap& map, Amino::Ptr<Amino::Array<Amino::Any>>& values)
                AMINO_ANNOTATE("Amino::Node");

            HYUU_DECL
                void map_set_items(HashMap& map AMINO_ANNOTATE("Amino::InOut outName=map_out"), const Amino::Ptr<Amino::Array<Amino::ulong_t>>& hashes, const Amino::Ptr<Amino::Array<Amino::Any>>& values)
                AMINO_ANNOTATE("Amino::Node");

            HYUU_DECL
                bool map_get_first(const HashMap& map, Amino::ulong_t& hash, Amino::Any& value)
                AMINO_ANNOTATE("Amino::Node");

        }  // namespace Map


        namespace Set {
            class AMINO_ANNOTATE("Amino::Class") HYUU_DECL HashSet {
            public:
                std::unordered_set<Amino::ulong_t, HashPass> m_set;

                HashSet() = default;

                HashSet(const HashSet & input) {
                    m_set = input.m_set;
                }

                ~HashSet() = default;
            };

            HYUU_DECL
                void set_add(HashSet& set AMINO_ANNOTATE("Amino::InOut outName=set_out"), const Amino::ulong_t& hash)
                AMINO_ANNOTATE("Amino::Node");

            HYUU_DECL
                void set_add(HashSet& set AMINO_ANNOTATE("Amino::InOut outName=set_out"), const Amino::Ptr<Amino::Array<Amino::ulong_t>>& hash)
                AMINO_ANNOTATE("Amino::Node");

            HYUU_DECL
                void set_remove(HashSet& set AMINO_ANNOTATE("Amino::InOut outName=set_out"), const Amino::ulong_t& hash)
                AMINO_ANNOTATE("Amino::Node");

            HYUU_DECL
                void set_remove(HashSet& set AMINO_ANNOTATE("Amino::InOut outName=set_out"), const Amino::Ptr<Amino::Array<Amino::ulong_t>>& hash)
                AMINO_ANNOTATE("Amino::Node");

            HYUU_DECL
				void set_hashes(const HashSet& set, Amino::Ptr<Amino::Array<Amino::ulong_t>>& hashes)
                AMINO_ANNOTATE("Amino::Node");

        }  // namespace Set


        // Container size  ===============================================================
        
        HYUU_DECL
            void container_size(const Map::HashMap& container, Amino::long_t& size)
            AMINO_ANNOTATE("Amino::Node");

        HYUU_DECL
            void container_size(const Set::HashSet& container, Amino::long_t& size)
            AMINO_ANNOTATE("Amino::Node");

        HYUU_DECL
            bool container_contains(const Map::HashMap& container, const Amino::ulong_t& hash)
            AMINO_ANNOTATE("Amino::Node");

        HYUU_DECL
            bool container_contains(const Set::HashSet& container, const Amino::ulong_t& hash)
            AMINO_ANNOTATE("Amino::Node");

        HYUU_DECL
            bool container_empty(const Map::HashMap& container)
            AMINO_ANNOTATE("Amino::Node");

        HYUU_DECL
            bool container_empty(const Set::HashSet& container)
            AMINO_ANNOTATE("Amino::Node");


		// Hash functions  ===============================================================
        HYUU_DECL
            Amino::ulong_t hash_value(const Amino::ulong_t& value)
			AMINO_ANNOTATE("Amino::Node");

        HYUU_DECL
            Amino::ulong_t hash_value(const Amino::long_t& value)
			AMINO_ANNOTATE("Amino::Node");
           
		HYUU_DECL
			Amino::ulong_t hash_value(const Amino::float_t& value)
			AMINO_ANNOTATE("Amino::Node");
        
        HYUU_DECL
			Amino::ulong_t hash_value(const Amino::String& value)
			AMINO_ANNOTATE("Amino::Node");

        HYUU_DECL
            Amino::ulong_t hash_value(const Bifrost::Math::float2& value)
            AMINO_ANNOTATE("Amino::Node");

		HYUU_DECL
			Amino::ulong_t hash_value(const Bifrost::Math::float3& value)
			AMINO_ANNOTATE("Amino::Node");

        HYUU_DECL
			Amino::ulong_t hash_value(const Bifrost::Math::float4& value)
			AMINO_ANNOTATE("Amino::Node");

        HYUU_DECL
            Amino::ulong_t hash_value(const Bifrost::Math::long2& value)
            AMINO_ANNOTATE("Amino::Node");

		HYUU_DECL
			Amino::ulong_t hash_value(const Bifrost::Math::long3& value)
			AMINO_ANNOTATE("Amino::Node");

        HYUU_DECL
			Amino::ulong_t hash_value(const Bifrost::Math::long4& value)
			AMINO_ANNOTATE("Amino::Node");
    }
}


AMINO_DECLARE_DEFAULT_CLASS(HYUU_DECL, Hyuu::Containers::Map::HashMap);
AMINO_DECLARE_DEFAULT_CLASS(HYUU_DECL, Hyuu::Containers::Set::HashSet);