package live.abhinav.facesearch.app;

import android.app.Activity;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.app.ProgressDialog;
import android.content.Context;
import android.graphics.Bitmap;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;
import org.json.JSONException;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;

public class MainActivity extends Activity implements Communicator {

    private static int count = 0;
    private ImageFragment imageFragment;
    private ResultFragment resultFragment;
    private String IP="192.188.10.4";
    private FragmentManager manager;
    Bitmap bp;

    private ProgressDialog dialog = null;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        requestWindowFeature(Window.FEATURE_NO_TITLE);
//        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
//                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main);


        resultFragment = new ResultFragment();
        manager = getFragmentManager();

        FragmentTransaction ft = manager.beginTransaction();
        ft = manager.beginTransaction();
        ft.add(R.id.parent, resultFragment, "B");
        ft.hide(resultFragment);
        ft.commit();

        ft = manager.beginTransaction();
        imageFragment = new ImageFragment();

        ft.add(R.id.parent, imageFragment, "A");
        ft.commit();
        Log.d("Lifecycle", "onCreate()");
    }

    public void callImageFragment(View view) {
        if (resultFragment.isVisible()) {

            //pop every thing from the backstack.
            FragmentManager.BackStackEntry first = manager.getBackStackEntryAt(0);
            manager.popBackStack(first.getId(), FragmentManager.POP_BACK_STACK_INCLUSIVE);

            FragmentTransaction ft = manager.beginTransaction();

            ft.hide(resultFragment);
            ft.show(imageFragment);
            ft.commit();
        }

        imageFragment.clickedButton();
    }

    // When user clicks button, calls AsyncTask.
    // Before attempting to fetch the URL, makes sure that there is a network connection.
    public void myClickHandler(View view) {

        FragmentTransaction ft;
        if (resultFragment.isHidden()) {
//            Toast.makeText(this, "was hidden", Toast.LENGTH_SHORT).show();
            ft = manager.beginTransaction();
            ft.hide(imageFragment);
            ft.show(resultFragment);
//                if(count==0)
            ft.addToBackStack("B");
            ft.commit();
        }

        count++;
        // Gets the URL from the UI's text field.
        String stringUrl = "http://" + IP + "/send/upload_media_test.php";
        ConnectivityManager connMgr = (ConnectivityManager)
                getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = connMgr.getActiveNetworkInfo();
        if (networkInfo != null && networkInfo.isConnected()) {

            dialog = ProgressDialog.show(MainActivity.this, "", "Uploading file...", true);
            new DownloadWebpageTask().execute(stringUrl);
        } else {
//            resultTV.setText("No network connection available.");
        }
    }

    @Override
    public void respond(Bitmap bitmap) {
        bp = bitmap;
    }

    // Uses AsyncTask to create a task away from the main UI thread. This task takes a
    // URL string and uses it to create an HttpUrlConnection. Once the connection
    // has been established, the AsyncTask downloads the contents of the webpage as
    // an InputStream. Finally, the InputStream is converted into a string, which is
    // displayed in the UI by the AsyncTask's onPostExecute method.
    private class DownloadWebpageTask extends AsyncTask<String, Void, String> {
        @Override
        protected String doInBackground(String... urls) {

            // params comes from the execute() call: params[0] is the url.
            try {
//                return downloadUrl(urls[0], "" + getFilesDir().getPath() + getPackageName() + "/files/image.jpg");
                Log.d("Lifecycle",urls[0]);
                return downloadUrl(urls[0], "/data/data/" + getPackageName() + "/files/image.jpg");
            } catch (IOException e) {
                Log.d("Lifecycle","Unable to retrieve web page. URL may be invalid.");
                Log.d("Lifecycle",urls[0]);
                return "Unable to retrieve web page. URL may be invalid.";
            }
        }

        // onPostExecute displays the results of the AsyncTask.
        @Override
        protected void onPostExecute(String result) {
//            resultTV.setText(result);
            try {
                resultFragment.showResults(result, bp);
            } catch (JSONException e) {
                e.printStackTrace();
            }
            dialog.dismiss();

        }
    }

    // Given a URL, establishes an HttpUrlConnection and retrieves
// the web page content as a InputStream, which it returns as
// a string.
    private String downloadUrl(String myurl, String sourceFileUri) throws IOException {
        InputStream is = null;
        HttpURLConnection conn;
        DataOutputStream dos;

        String lineEnd = "\r\n";
        String twoHyphens = "--";
        String boundary = "*****";
        int bytesRead, bytesAvailable, bufferSize;
        byte[] buffer;
        int maxBufferSize = 1 * 1024 * 1024;
        File sourceFile = new File(sourceFileUri);
        if (!sourceFile.isFile()) {
            Log.e("uploadFile", "Source File Does not exist");
            return null;
        }

        // Only display the first 500 characters of the retrieved
        // web page content.
        int len = 500;

        try {
            FileInputStream fileInputStream = new FileInputStream(sourceFile);

            URL url = new URL(myurl);

            conn = (HttpURLConnection) url.openConnection();
            conn.setReadTimeout(0);
            conn.setConnectTimeout(0);
            conn.setDoInput(true); // Allow Inputs
            conn.setDoOutput(true); // Allow Outputs
            conn.setUseCaches(false); // Don't use a Cached Copy
            conn.setRequestMethod("POST");
            conn.setRequestProperty("Connection", "Keep-Alive");
            conn.setRequestProperty("ENCTYPE", "multipart/form-data");
            conn.setRequestProperty("Content-Type", "multipart/form-data;boundary=" + boundary);
            conn.setRequestProperty("uploaded_file", sourceFileUri);
            conn.setRequestProperty("username", "Abhinav Anand");
            conn.setRequestProperty("quality", "Coool");

            //Send request
            dos = new DataOutputStream(conn.getOutputStream());
            dos.writeBytes(twoHyphens + boundary + lineEnd);
            dos.writeBytes("Content-Disposition: form-data; name=\"uploaded_file\";filename=\"" + sourceFileUri + "\"" + lineEnd);
            dos.writeBytes(lineEnd);
            bytesAvailable = fileInputStream.available(); // create a buffer of  maximum size
            bufferSize = Math.min(bytesAvailable, maxBufferSize);
            buffer = new byte[bufferSize];

            // read file and write it into form...
            bytesRead = fileInputStream.read(buffer, 0, bufferSize);

            while (bytesRead > 0) {
                dos.write(buffer, 0, bufferSize);
                bytesAvailable = fileInputStream.available();
                bufferSize = Math.min(bytesAvailable, maxBufferSize);
                bytesRead = fileInputStream.read(buffer, 0, bufferSize);
            }
            // send multipart form data necesssary after file data...
            dos.writeBytes(lineEnd);
            dos.writeBytes(twoHyphens + boundary + twoHyphens + lineEnd);
////////////////////////////////////////////////////

            // Starts the query
            conn.connect();
            int response = conn.getResponseCode();
            Log.d("Lifecycle", "The response is: " + response);
            is = conn.getInputStream();

            // Convert the InputStream into a string
            String contentAsString = readIt(is, len);

            fileInputStream.close();
            dos.flush();
            dos.close();

            return contentAsString;

            // Makes sure that the InputStream is closed after the app is
            // finished using it.
        } finally {
            if (is != null) {
                is.close();
            }
        }
    }

    // Reads an InputStream and converts it to a String.
    public String readIt(InputStream stream, int len) throws IOException {
        Reader reader;
        reader = new InputStreamReader(stream, "UTF-8");
        char[] buffer = new char[len];
        reader.read(buffer);
        return new String(buffer);
    }
}