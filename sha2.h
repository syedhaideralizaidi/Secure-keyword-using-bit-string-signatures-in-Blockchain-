#ifndef SHA2_H
#define SHA2_H

#include <string>

std::string sha224(const std::string & input);
std::string sha256(const std::string & input);
std::string sha384(const std::string & input);
std::string sha512(const std::string & input);

template<typename T1, typename T2>
constexpr auto sha2_shfr(T1 x, T2  n) { return (x >> n); }
template<typename T1, typename T2>
constexpr auto sha2_rotr(T1 x, T2  n) { return ((x >> n) | (x << ((sizeof(x) << 3) - n))); }
template<typename T1, typename T2>
constexpr auto sha2_rotl(T1 x, T2  n) { return ((x << n) | (x >> ((sizeof(x) << 3) - n))); }
template<typename T1, typename T2, typename T3>
constexpr auto sha2_ch(T1 x, T2  y, T3  z) { return ((x & y) ^ (~x & z)); }
template<typename T1, typename T2, typename T3>
constexpr auto sha2_maj(T1 x, T2  y, T3  z) { return ((x & y) ^ (x & z) ^ (y & z)); }

template<typename T>
constexpr auto sha256_f1(T x) {
	return (sha2_rotr(x, 2) ^ sha2_rotr(x, 13) ^ sha2_rotr(x, 22));
}

template<typename T>
constexpr auto sha256_f2(T x) {
	return (sha2_rotr(x, 6) ^ sha2_rotr(x, 11) ^ sha2_rotr(x, 25));
}

template<typename T>
constexpr auto sha256_f3(T x) {
	return (sha2_rotr(x, 7) ^ sha2_rotr(x, 18) ^ sha2_shfr(x, 3));
}

template<typename T>
constexpr auto sha256_f4(T x) {
	return (sha2_rotr(x, 17) ^ sha2_rotr(x, 19) ^ sha2_shfr(x, 10));
}


template <typename T>
constexpr auto sha512_f1(T x) {
	return (sha2_rotr(x, 28) ^ sha2_rotr(x, 34) ^ sha2_rotr(x, 39));
}

template <typename T>
constexpr auto sha512_f2(T x) {
	return (sha2_rotr(x, 14) ^ sha2_rotr(x, 18) ^ sha2_rotr(x, 41));
}

template <typename T>
constexpr auto sha512_f3(T x) {
	return (sha2_rotr(x, 1) ^ sha2_rotr(x, 8) ^ sha2_shfr(x, 7));
}

template <typename T>
constexpr auto sha512_f4(T x) {
	return (sha2_rotr(x, 19) ^ sha2_rotr(x, 61) ^ sha2_shfr(x, 6));
}

template <typename T1, typename T2>
constexpr void sha2_unpack32(T1 x, T2 * str)
{
	str[3] = static_cast<std::uint8_t>(x);
	str[2] = static_cast<std::uint8_t>(x >> 8);
	str[1] = static_cast<std::uint8_t>(x >> 16);
	str[0] = static_cast<std::uint8_t>(x >> 24);
}

template <typename T1, typename T2>
constexpr void sha2_pack32(T1 * str, T2 x)
{
	*x = (static_cast<std::uint32_t>(str[3])) | ((static_cast<std::uint32_t>(str[2])) << 8) | ((static_cast<std::uint32_t>(str[1])) << 16) | ((static_cast<std::uint32_t>(str[0])) << 24);
}

template <typename T1, typename T2>
constexpr void sha2_unpack64(T1 x, T2 * str)
{
	str[7] = static_cast<std::uint8_t>(x);
	str[6] = static_cast<std::uint8_t>(x >> 8);
	str[5] = static_cast<std::uint8_t>(x >> 16);
	str[4] = static_cast<std::uint8_t>(x >> 24);
	str[3] = static_cast<std::uint8_t>(x >> 32);
	str[2] = static_cast<std::uint8_t>(x >> 40);
	str[1] = static_cast<std::uint8_t>(x >> 48);
	str[0] = static_cast<std::uint8_t>(x >> 56);
}

template <typename T1, typename T2>
constexpr void sha2_pack64(T1 * str, T2 x)
{
	*x = (static_cast<std::uint64_t>(str[7])) | ((static_cast<std::uint64_t>(str[6])) << 8) | ((static_cast<std::uint64_t>(str[5])) << 16) | ((static_cast<std::uint64_t>(str[4])) << 24)
		| ((static_cast<std::uint64_t>(str[3])) << 32) | ((static_cast<std::uint64_t>(str[2])) << 40) | ((static_cast<std::uint64_t>(str[1])) << 48) | ((static_cast<std::uint64_t>(str[0])) << 56);
}


class SHA2 {
	virtual void init() = 0;
	virtual void update(const char *message, unsigned int len) = 0;
	virtual void final(unsigned char *digest) = 0;
};

class SHA256 : public SHA2
{
protected:
	const static std::uint32_t sha256_k[];
	static const unsigned int SHA224_256_BLOCK_SIZE = (512 / 8);

public:
	void init() override;
	void update(const char *message, unsigned int len) override;
	void final(unsigned char *digest) override;
	static const unsigned int DIGEST_SIZE = (256 / 8);

protected:
	template <typename T>
	void transform(const T *message, unsigned int block_nb)
	{
		std::uint32_t w[64];
		std::uint32_t wv[8];
		std::uint32_t t1, t2;
		int i;
		int j;
		for (i = 0; i < (int)block_nb; i++) {
			auto sub_block = message + (i << 6);
			for (j = 0; j < 16; j++) {
				sha2_pack32(&sub_block[j << 2], &w[j]);
			}
			for (j = 16; j < 64; j++) {
				w[j] = sha256_f4(w[j - 2]) + w[j - 7] + sha256_f3(w[j - 15]) + w[j - 16];
			}
			for (j = 0; j < 8; j++) {
				wv[j] = m_h[j];
			}
			for (j = 0; j < 64; j++) {
				t1 = wv[7] + sha256_f2(wv[4]) + sha2_ch(wv[4], wv[5], wv[6])
					+ sha256_k[j] + w[j];
				t2 = sha256_f1(wv[0]) + sha2_maj(wv[0], wv[1], wv[2]);
				wv[7] = wv[6];
				wv[6] = wv[5];
				wv[5] = wv[4];
				wv[4] = wv[3] + t1;
				wv[3] = wv[2];
				wv[2] = wv[1];
				wv[1] = wv[0];
				wv[0] = t1 + t2;
			}
			for (j = 0; j < 8; j++) {
				m_h[j] += wv[j];
			}
		}
	}

	unsigned int m_total_len = 0;
	unsigned int m_len = 0;
	unsigned char m_block[2 * SHA224_256_BLOCK_SIZE];
	std::uint32_t m_h[8];// = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
};

class SHA224 : public SHA256
{
public:
	void init() override;
	void update(const char *message, unsigned int len) override;
	void final(unsigned char *digest) override;
	static const unsigned int DIGEST_SIZE = (224 / 8);
};

class SHA512 : public SHA2
{
protected:
	const static std::uint64_t sha512_k[];
	static const unsigned int SHA384_512_BLOCK_SIZE = (1024 / 8);

public:
	void init() override;
	void update(const char *message, unsigned int len) override;
	void final(unsigned char *digest) override;
	static const unsigned int DIGEST_SIZE = (512 / 8);

protected:
	template <typename T>
	void transform(const T *message, unsigned int block_nb)
	{
		std::uint64_t w[80];
		std::uint64_t wv[8];
		std::uint64_t t1, t2;
		int i, j;
		for (i = 0; i < (int)block_nb; i++) {
			auto sub_block = message + (i << 7);
			for (j = 0; j < 16; j++) {
				sha2_pack64(&sub_block[j << 3], &w[j]);
			}
			for (j = 16; j < 80; j++) {
				w[j] = sha512_f4(w[j - 2]) + w[j - 7] + sha512_f3(w[j - 15]) + w[j - 16];
			}
			for (j = 0; j < 8; j++) {
				wv[j] = m_h[j];
			}
			for (j = 0; j < 80; j++) {
				t1 = wv[7] + sha512_f2(wv[4]) + sha2_ch(wv[4], wv[5], wv[6])
					+ sha512_k[j] + w[j];
				t2 = sha512_f1(wv[0]) + sha2_maj(wv[0], wv[1], wv[2]);
				wv[7] = wv[6];
				wv[6] = wv[5];
				wv[5] = wv[4];
				wv[4] = wv[3] + t1;
				wv[3] = wv[2];
				wv[2] = wv[1];
				wv[1] = wv[0];
				wv[0] = t1 + t2;
			}
			for (j = 0; j < 8; j++) {
				m_h[j] += wv[j];
			}

		}
	}

	unsigned int m_total_len = 0;
	unsigned int m_len = 0;
	unsigned char m_block[2 * SHA384_512_BLOCK_SIZE];
	std::uint64_t m_h[8];
};

class SHA384 : public SHA512
{
public:
	void init() override;
	void update(const char *message, unsigned int len) override;
	void final(unsigned char *digest) override;
	static const unsigned int DIGEST_SIZE = (384 / 8);
};

#endif