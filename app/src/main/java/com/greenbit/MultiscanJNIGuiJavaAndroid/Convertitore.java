package com.greenbit.MultiscanJNIGuiJavaAndroid;

public class Convertitore {
	/**
     * Scrive un valore long dentro un buffer binario in codifica Little endian
     * (MSB last)
     *
     * @param ValueToWrite
     * @param buff
     * @param offset
	 * @throws Exception 
     */
    public static void WriteUInt32ToBinary(long ValueToWrite, byte[] buff, int offset) throws Exception {
        try {
            buff[offset] = (byte) (ValueToWrite & 0xFF);
            buff[offset + 1] = (byte) ((ValueToWrite & 0xFF00) >> 8);
            buff[offset + 2] = (byte) ((ValueToWrite & 0xFF0000) >> 16);
            buff[offset + 3] = (byte) ((ValueToWrite & 0xFF000000) >> 24);
        } catch (Exception ex) {
            throw (ex);
        }
    }

    /**
     * legge un valore long da un buffer binario in codifica Little endian (MSB
     * last)
     *
     * @param buff
     * @param offset
     * @return
     * @throws Exception 
     */
    // ver 1.1.2: bug nelle funzioni
    public static long ReadUInt32FromBinary(byte[] buff, int offset) throws Exception {
        try {
            long ValueToRead;
            long app;

            if (buff[offset] >= 0 ) app = buff[offset];
            else app = 255 + buff[offset] + 1;
            ValueToRead = app;
            if (buff[offset + 1] >= 0 ) app = buff[offset + 1];
            else app = 255 + buff[offset + 1] + 1;
            ValueToRead += (long) ((app & 0x000000FF) << 8);
            if (buff[offset + 2] >= 0 ) app = buff[offset + 2];
            else app = 255 + buff[offset + 2] + 1;
            ValueToRead += (long) ((app & 0x000000FF) << 16);
            if (buff[offset + 3] >= 0 ) app = buff[offset + 3];
            else app = 255 + buff[offset + 3] + 1;
            ValueToRead += (long) ((app & 0x000000FF) << 24);
            return ValueToRead;
        } catch (Exception ex) {
            throw (ex);
        }
    }
    // end ver 1.1.2: bug nelle funzioni

    /**
     * Scrive un valore int dentro un buffer binario in codifica Little endian
     * (MSB last)
     *
     * @param ValueToWrite
     * @param buff
     * @param offset
     * @throws Exception 
     */
    public static void WriteUInt16ToBinary(int ValueToWrite, byte[] buff, int offset) throws Exception {
        try {
            buff[offset] = (byte) (ValueToWrite & 0xFF);
            buff[offset + 1] = (byte) ((ValueToWrite & 0xFF00) >> 8);
        } catch (Exception ex) {
            throw (ex);
        }
    }

    /**
     * legge un valore int da un buffer binario in codifica Little endian (MSB
     * last)
     *
     * @param buff
     * @param offset
     * @return
     * @throws Exception 
     */
    // ver 1.1.2: bug nelle funzioni
    public static int ReadUInt16FromBinary(byte[] buff, int offset) throws Exception {
        try {
            int ValueToRead;
            int app;

            if (buff[offset] >= 0 ) app = buff[offset];
            else app = 255 + buff[offset] + 1;
            ValueToRead = app;
            if (buff[offset + 1] >= 0 ) app = buff[offset + 1];
            else app = 255 + buff[offset + 1] + 1;
            ValueToRead += (int) ((app & 0x000000FF) << 8);
            return ValueToRead;
        } catch (Exception ex) {
            throw (ex);
        }
    }
    // end ver 1.1.2: bug nelle funzioni

    /**
     * Scrive un valore boolean dentro un buffer binario in codifica Little
     * endian (MSB last) memorizzandolo dentro 2 byte
     *
     * @param ValueToWrite
     * @param buff
     * @param offset
     * @throws Exception 
     */
    public static void WriteBoolToBinaryUInt16(boolean ValueToWrite, byte[] buff, int offset) throws Exception {
        try {
            if (ValueToWrite) {
                buff[offset + 1] = 1;
            } else {
                buff[offset + 1] = 0;
            }
            buff[offset] = 0;
        } catch (Exception ex) {
            throw (ex);
        }
    }

    /**
     * legge un valore bool memorizzato in 2 byte da un buffer binario in
     * codifica Little endian (MSB last)
     *
     * @param buff
     * @param offset
     * @return
     * @throws Exception 
     */
    public static boolean ReadBoolFromBinaryUInt16(byte[] buff, int offset) throws Exception {
        try {
            boolean ValueToRead;
            ValueToRead = buff[offset + 1] != 0;
            return ValueToRead;
        } catch (Exception ex) {
            throw (ex);
        }
    }

    /**
     * Scrive un valore boolean dentro un buffer binario in codifica Little
     * endian (MSB last) memorizzandolo dentro 1 byte
     *
     * @param ValueToWrite
     * @param buff
     * @param offset
     * @throws Exception 
     */
    public static void WriteBoolToBinaryUInt8(boolean ValueToWrite, byte[] buff, int offset) throws Exception {
        try {
            if (ValueToWrite) {
                buff[offset] = 1;
            } else {
                buff[offset] = 0;
            }
        } catch (Exception ex) {
            throw (ex);
        }
    }

    /**
     * legge un valore bool memorizzato in 1 byte da un buffer binario in
     * codifica Little endian (MSB last)
     *
     * @param buff
     * @param offset
     * @return
     * @throws Exception 
     */
    public static boolean ReadBoolFromBinaryUInt8(byte[] buff, int offset) throws Exception {
        try {
            return (0 != buff[offset]);
        } catch (Exception ex) {
            throw (ex);
        }
    }

    /**
     * Converte una stringa (supposto che sia ASCII) in binario
     *
     * @param x
     * @param maxChars
     * @param buff
     * @param offset
     * @throws Exception 
     */
    // ver 1.1.2: bug nelle funzioni
    public static void WriteStringToBinary(String x, int maxChars, byte[] buff, int offset) throws Exception {
        try {
            int charsToRead = x.length() > maxChars ? maxChars : x.length();
            for (int i = 0; i < (maxChars + 1); i++) {
                buff[i + offset] = 0;
            }
            for (int i = 0; i < charsToRead; i++) {
                buff[i + offset] = (byte) ((int) x.charAt(i));
            }
        } catch (Exception ex) {
            throw (ex);
        }
    }

    /**
     * Legge una stringa (null terminated) da un file binario
     * @param maxChars
     * @param buff
     * @param offset
     * @return 
     * @throws Exception 
     */
    public static String ReadStringFromBinary(int maxChars, byte[] buff, int offset) throws Exception 
    {        
        try {
            String ValueToRead = "";
            for (int i = 0; i < maxChars; i ++)
            {
                if (buff[i + offset] == 0) return ValueToRead;
                char k;
                k = (char)((int)buff[i + offset]);
                ValueToRead += k;
            }
            return ValueToRead;
        } catch (Exception ex) {
            throw (ex);
        }
    }
    // end ver 1.1.2: bug nelle funzioni
}
