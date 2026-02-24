import { useEffect, useRef } from 'react';

/**
 * rAF loop hook with proper cleanup to prevent frame accumulation.
 * The draw function ref is updated on each render without restarting the loop.
 */
export function useAnimationLoop(draw: () => void, deps: React.DependencyList = []) {
  const rafRef = useRef<number>(0);
  const drawRef = useRef(draw);
  drawRef.current = draw;

  useEffect(() => {
    const loop = () => {
      drawRef.current();
      rafRef.current = requestAnimationFrame(loop);
    };
    rafRef.current = requestAnimationFrame(loop);
    return () => cancelAnimationFrame(rafRef.current);
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, deps);
}
