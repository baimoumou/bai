#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include"Huffmantree.h"
#include<assert.h>
#include<Windows.h>
#include"Heap.h"
struct CharInfo
{
	char _ch;
	long long _count;
	string _code;
	bool operator !=(const CharInfo& info)
	{
		return _count != info._count;
	}
	CharInfo operator+(const CharInfo& info)
	{
		CharInfo ret;
		ret._count= _count + info._count;
		return ret;
	}
	bool operator<(const CharInfo& info) const
	{
		return _count < info._count;
	}
};
class Filecompress
{
	typedef HuffmantreeNode<CharInfo> Node;
public:
	Filecompress()
	{
		for (size_t i = 0; i < 256; ++i)
		{
			_infos[i]._ch = i;
			_infos[i]._count = 0;
		}
	}
	//yasuo
	void Compress(const char*filename)
	{
		assert(filename);
		struct _HuffmanInfo
		{
			char _ch;
			long long _count;
		};
		FILE* fout = fopen(filename, "rb");
		cout << errno << endl;
		assert(fout);
		char ch = fgetc(fout);
		while  (!feof(fout))  //(ch!= EOF)
		{
			_infos[(unsigned char)ch]._count++;
			ch = fgetc(fout);
		}
		CharInfo invalid;
		invalid._count = 0;
		Huffmantree<CharInfo>tree(_infos, 256, invalid);	

		GenerateHuffmanCode(tree.GetRoot());
		string CompressFile = filename;
		CompressFile += ".Huffman";
		FILE*fin = fopen(CompressFile.c_str(),"wb");
		assert(fin);
		_HuffmanInfo Info;
		size_t size;
		for (size_t i = 0; i < 256; i++)
		{
			if (_infos[i]._count)
			{
				Info._ch = _infos[i]._ch;
				Info._count = _infos[i]._count;
				size = fwrite(&Info, sizeof(_HuffmanInfo), 1, fin);
				assert(size==1);
			}
		}
		Info._count = 0;
		size = fwrite(&Info, sizeof(_HuffmanInfo), 1, fin);
		//½áÊøÅÐ¶Ï
		assert(size == 1);
		char value = 0;
		int count = 0;
		fseek(fout, 0, SEEK_SET);
		ch = fgetc(fout);
		while (!feof(fout))
		{
			string& code = _infos[(unsigned char)ch]._code;
			for (size_t i = 0; i < code.size(); ++i)
			{
				value <<= 1;
				if (code[i] == '1')
				{
					value |= 1;
				}
				else
				{
					value |= 0;
				}
				++count;
				if (count == 8)
				{
					fputc(value, fin);
					value = 0;
					count = 0;
				}
			}
			ch = fgetc(fout);
		}
		if (count != 0)
		{
			value <<= (8 - count);
			fputc(value, fin);
		}
		fclose(fin);
		fclose(fout);
	}
public:
	//jieyasuo
	void Uncompress(const char* filename)
	{
		assert(filename);
		struct _HuffmanInfo
		{
			char _ch;
			long long _count;
		};
		string UncompressFile = filename;
		size_t pos = UncompressFile.rfind('.');
		assert(pos != string::npos);
		UncompressFile = UncompressFile.substr(0, pos);
		UncompressFile += ".UnHuffman.jpg";
		FILE*fin = fopen(UncompressFile.c_str(), "wb");
		size_t size = 0;
		assert(fin);
		FILE*fout = fopen(filename, "rb");
		while (1)
		{
			_HuffmanInfo info;
			size = fread(&info,sizeof(_HuffmanInfo),1,fout);
			assert(size == 1);
			if (info._count)
			{
				_infos[(unsigned char)info._ch]._ch = info._ch;
				_infos[(unsigned char)info._ch]._count = info._count;
			}
			else
			{
				break;
			}
		}
		CharInfo invalid;
		invalid._count = 0;
		Huffmantree<CharInfo> tree(_infos, 256, invalid);

		Node* root = tree.GetRoot();
		GenerateHuffmanCode(tree.GetRoot());
		long long charcount = root->_w._count;

		char value;
		value = fgetc(fout);
		Node* cur = root;
 		int count = 0;
		while (charcount)
		{
			for (int pos = 7; pos >= 0; --pos)
			{
				if (value &(1 << pos))
				{
					cur = cur->right;
				}
				else
				{
					cur = cur->left;
				}
				if (cur->left == NULL && cur->right == NULL)
				{
					fputc(cur->_w._ch, fin);
					cur = root;
					--charcount;
					if (charcount == 0)
					{
						break;
					}
				}
			}
			value = fgetc(fout);
		}
		fclose(fin);
		fclose(fout);
	}
	public:
		void GenerateHuffmanCode(Node*root)
		{
			if (root == NULL)
				return;
			if (root->left == NULL&&root->right == NULL)
			{
				string& code = _infos[(unsigned char)root->_w._ch]._code;
				Node*cur = root;
				Node*parent = cur->parent;
				while (parent)
				{
					if (parent->left == cur)
						code.push_back('0');
					if (parent->right == cur)
						code.push_back('1');
					cur = parent;
					parent = parent->parent;
				}
				reverse(code.begin(), code.end());
			}
			GenerateHuffmanCode(root->left);
			GenerateHuffmanCode(root->right);
		}
protected:
	CharInfo _infos[256];
};
void test()
{
	Filecompress fc;
	fc.Compress("22.jpg");
	fc.Uncompress("22.jpg.Huffman");
}

