package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.Manifest;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.print.PrintAttributes;
import android.print.PrintDocumentAdapter;
import android.print.PrintManager;
import android.util.Log;
import android.widget.Button;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.itextpdf.text.BaseColor;
import com.itextpdf.text.Chunk;
import com.itextpdf.text.Document;
import com.itextpdf.text.DocumentException;
import com.itextpdf.text.Element;
import com.itextpdf.text.Font;
import com.itextpdf.text.PageSize;
import com.itextpdf.text.Paragraph;
import com.itextpdf.text.pdf.BaseFont;
import com.itextpdf.text.pdf.PdfWriter;
import com.itextpdf.text.pdf.draw.LineSeparator;
import com.karumi.dexter.Dexter;
import com.karumi.dexter.PermissionToken;
import com.karumi.dexter.listener.PermissionDeniedResponse;
import com.karumi.dexter.listener.PermissionGrantedResponse;
import com.karumi.dexter.listener.PermissionRequest;
import com.karumi.dexter.listener.single.PermissionListener;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URL;

import pl.droidsonroids.gif.GifImageView;

public class PrinterActivity extends AppCompatActivity {
    private Button print;
    private GifImageView gifImageView;
    //font
    BaseColor colorAccent = new BaseColor(0, 153, 204, 255);
    float sectonHeaderSize = 10.0f;
    float titleSize = 5.0f;
    float valueFontSize = 8.0f;
//    float sectonHeaderSize = 30.0f;
//    float titleSize = 20.0f;
//    float valueFontSize = 26.0f;


    private BaseFont fontName;
    private Document document;
    private byte[] byteArray;

    private String[] titles = {"BIPPIIS NUMBER", "FULL NAME", "PHONE", "CATEGORY", "PERMANENT HOUSE ADDRESS", "NEXT OF KIN NAME", "NEXT OF KIN NUMBER",
            "RELATIONSHIP WITH NEXT OF KIN", "NEXT OF KIN ADDRESS", "DATE OF FIRST APPOINTMENT", "RETIREMENT DATE",
            "YEARS SERVED", "CLASSIFICATION", "RANK", "GRADE LEVEL STOPPED", "DATE OF LAST PROMOTION",
            "LOCAL GOVERNMENT OF RETIREMENT", "LOCAL GOVERNMENT OF ORIGIN"
    };
    private String[] values = {"Demo Text", "Demo Text", "Demo Text", "Demo Text", "Demo Text", "Demo Text", "Demo Text", "Demo Text", "Demo Text"};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_printer);
        print = findViewById(R.id.print);
        gifImageView = findViewById(R.id.gif);


        try {
/*
            new Thread(() -> {
                //Do whatever
                Drawable drawable = getResources().getDrawable(R.drawable.logo);
                Bitmap bitmap = drawableToBitmap(drawable);
                ByteArrayOutputStream stream = new ByteArrayOutputStream();
                bitmap.compress(Bitmap.CompressFormat.PNG, 100, stream);
                byteArray = stream.toByteArray();

            }).start();
*/

            //custom font
            fontName = BaseFont.createFont("assets/fonts/HVD Fonts - BrandonText-Regular.otf", "UTF-8", BaseFont.EMBEDDED);

        } catch (DocumentException e) {
            e.printStackTrace();
            Log.d("fingerprint", e.getMessage());
        } catch (IOException e) {
            e.printStackTrace();
        }

        print.setOnClickListener(v -> {
            Toast.makeText(PrinterActivity.this, "createPDFFile almost called", Toast.LENGTH_SHORT).show();
            createPDFFile(Common.getAppPath(PrinterActivity.this) + "test_pdf.pdf");

        });

        print.setEnabled(true);
        Dexter.withActivity(this).withPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE).withListener(new PermissionListener() {
            @Override
            public void onPermissionGranted(PermissionGrantedResponse response) {
                print.setEnabled(true);
                print.setOnClickListener(v -> {
                    Toast.makeText(PrinterActivity.this, "createPDFFile almost called", Toast.LENGTH_SHORT).show();
                    createPDFFile(Common.getAppPath(PrinterActivity.this) + "test_pdf.pdf");

                });
            }

            @Override
            public void onPermissionDenied(PermissionDeniedResponse response) {
                print.setEnabled(false);
                print.setText("Printer Permission Denied");
            }

            @Override
            public void onPermissionRationaleShouldBeShown(PermissionRequest permission, PermissionToken token) {

            }
        });

    }

    private void createPDFFile(String path) {
        Toast.makeText(this, "createPDFFile called", Toast.LENGTH_SHORT).show();
        if (new File(path).exists())
            new File(path).delete();

        try {
            document = new Document();
            //save
            PdfWriter.getInstance(document, new FileOutputStream(path));
            //open
            document.open();
            document.setPageSize(PageSize.A4);
            document.addCreationDate();
            document.addAuthor("BIPPIIS");
            document.addCreator("Benue State Pensions Board");


            //section title
            Font titleFont = new Font(fontName, sectonHeaderSize, Font.NORMAL, BaseColor.BLACK);
            addNewItem(document, "BIPPIIS Enrollment Successful", Element.ALIGN_CENTER, titleFont);

            for (String title : titles) {
                addSection(title, values[0]);
            }
            document.close();
         /*   // read existing pdf to add watermark
            PdfReader reader = new PdfReader(Common.getAppPath(PrinterActivity.this) + "test_pdf.pdf");
            PdfStamper stamper = new PdfStamper(reader, new FileOutputStream(Common.getAppPath(PrinterActivity.this) + "test_pdf.pdf"));

            // image watermark
//            Image img = Image.getInstance(String.valueOf(getResources().getDrawable(R.drawable.logo)));
            Image img = Image.getInstance(byteArray);
//            Image img = Image.getInstance(getResources("/logo.png"));
            float w = img.getScaledWidth();
            float h = img.getScaledHeight();


            // properties
            PdfContentByte over;
            Rectangle pagesize;
            float x, y;
            // loop over every page
            int n = reader.getNumberOfPages();
            for (int i = 1; i <= n; i++) {

                // get page size and position
                pagesize = reader.getPageSizeWithRotation(i);
                x = (pagesize.getLeft() + pagesize.getRight()) / 2;
                y = (pagesize.getTop() + pagesize.getBottom()) / 2;
                over = stamper.getOverContent(i);
                over.saveState();

                // set transparency
                PdfGState state = new PdfGState();
                state.setFillOpacity(0.2f);
                over.setGState(state);

                // add watermark text and image

                over.addImage(img, w, 0, 0, h, x - (w / 2), y - (h / 2));


                over.restoreState();
            }
            stamper.close();
            reader.close();
*/

            Toast.makeText(this, "Success", Toast.LENGTH_SHORT).show();
            printPDF();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            Log.d("fingerprint", "File not found " + e.getMessage());
        } catch (DocumentException e) {
            e.printStackTrace();
            Log.d("fingerprint", "DocumentException " + e.getMessage());

        }
    }

    private static URL getResources(String name) {
        return PrinterActivity.class.getResource(name);
    }

    private void printPDF() {
        PrintManager printManager = (PrintManager) getSystemService(Context.PRINT_SERVICE);


        try {
            PrintDocumentAdapter printDocumentAdapter = new PdfDocumentAdapter(PrinterActivity.this, Common.getAppPath(PrinterActivity.this) + "test_pdf.pdf");
            printManager.print("Document", printDocumentAdapter, new PrintAttributes.Builder().build());
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    private void addSection(String title, String value) {

        Font orderNumberFont = new Font(fontName, titleSize, Font.NORMAL, colorAccent);
        try {
            //add heading/title
            addNewItem(document, title, Element.ALIGN_LEFT, orderNumberFont);

            //add value
            Font orderNumberValue = new Font(fontName, valueFontSize, Font.NORMAL, BaseColor.BLACK);
            addNewItem(document, value, Element.ALIGN_LEFT, orderNumberValue);

            addLineSeparator(document);

        } catch (DocumentException e) {
            e.printStackTrace();
        }


    }

    private void addLineSeparator(Document document) throws DocumentException {
        LineSeparator lineSeparator = new LineSeparator();
        lineSeparator.setLineColor(new BaseColor(0, 0, 0, 68));
        addLineSpace(document);
        document.add(new Chunk(lineSeparator));
        addLineSpace(document);

    }

    private void addLineSpace(Document document) throws DocumentException {
        document.add(new Paragraph(""));
    }


    private void addNewItem(Document document, String section_title, int align, Font font) throws DocumentException {
        Chunk chunk = new Chunk(section_title, font);
        Paragraph paragraph = new Paragraph(chunk);
        paragraph.setAlignment(align);
        document.add(paragraph);
    }

    public static Bitmap drawableToBitmap(Drawable drawable) {
        if (drawable instanceof BitmapDrawable) {
            return ((BitmapDrawable) drawable).getBitmap();
        }

        final int width = !drawable.getBounds().isEmpty() ? drawable
                .getBounds().width() : drawable.getIntrinsicWidth();

        final int height = !drawable.getBounds().isEmpty() ? drawable
                .getBounds().height() : drawable.getIntrinsicHeight();

        final Bitmap bitmap = Bitmap.createBitmap(width <= 0 ? 1 : width,
                height <= 0 ? 1 : height, Bitmap.Config.ARGB_8888);

        Log.v("Bitmap width - Height :", width + " : " + height);
        Canvas canvas = new Canvas(bitmap);
        drawable.setBounds(0, 0, canvas.getWidth(), canvas.getHeight());
        drawable.draw(canvas);

        return bitmap;
    }
}
