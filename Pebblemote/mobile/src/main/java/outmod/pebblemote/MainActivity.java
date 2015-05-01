package outmod.pebblemote;

import android.content.Context;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

import com.getpebble.android.kit.PebbleKit;
import com.getpebble.android.kit.util.PebbleDictionary;
import com.google.android.gms.cast.Cast;
import com.google.android.gms.cast.CastDevice;
import com.google.android.gms.cast.CastMediaControlIntent;
import com.google.android.gms.cast.CastStatusCodes;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.plus.Plus;

import java.util.UUID;


public class MainActivity extends ActionBarActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        //default initialization parameters
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //parameter setup
        //Pebble setup
        final UUID uuid = UUID.fromString("3f31b449-d008-4394-9626-cca6566a6f67");
        final boolean connected = PebbleKit.isWatchConnected(getApplicationContext());
        Log.i(getLocalClassName(), "Pebble is " + (connected ? "connected" : "not connected"));

        //Google cast API setup
        //MediaRouter mediaRouter =
        Button testButton = (Button)findViewById(R.id.testButton);
        testButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //If pebble is connected, attempt to send a notification
                if(connected){
                    PebbleKit.startAppOnPebble(getApplicationContext(),uuid);
                }
            }
        });

        PebbleKit.registerReceivedDataHandler(this, new PebbleKit.PebbleDataReceiver(uuid) {
            @Override
            public void receiveData(Context context, int i, PebbleDictionary pebbleDictionary) {
                Log.i(getLocalClassName(),"Got a message!");
            }
        });

    }

    @Override
    public void onStart(){

    }

    @Override
    public void onStop(){

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
